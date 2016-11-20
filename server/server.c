/*
*openSM server prototype
*(c) 2016 Zachary James Schlotman
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <opensm_libio.h>
#include <libmisc.h>
#include <libuser.h>
void usage(char *arg){
	printf("%s config_file\n",arg);
}
int main(int argc,char *argv[]){
	if(argc != 2){
		usage(argv[0]);
		return -1;
	}
	FILE *f = fopen(argv[1],"r");
	if(!f){
		perror("Error opening file");
		return -1;
	}
	struct __server_cfg *cfg = parse_cfg(f);
	fclose(f);
	printf("openSM Server V0.1-Alpha\n");
	printf("Starting up\n");
	int opt = 1;
	int end = 0x7F;
	int es_socket,addrlen,new_socket,client_socket[cfg->maxconnections],max_clients = cfg->maxconnections;
	int max_sd;
	struct sockaddr_in address;
	char *buffer = malloc(1024);
	fd_set rfds;
	printf("Initializing values\n");
	for(int i = 0; i < max_clients;i++)
		client_socket[i] = 0;
	if((es_socket = socket(AF_INET,SOCK_STREAM,0)) == 0){
		perror("Failed to establish socket");
		return -1;
	}
	printf("Seting socket options\n");
	if(setsockopt(es_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt)) < 0){
		perror("Failed to set socket options");
		return -1;
	}
	address.sin_family = AF_INET;
	address.sin_port = htons(2718);
	address.sin_addr.s_addr = INADDR_ANY;;
	printf("Binding port\n");
	if(bind(es_socket,(struct sockaddr *)&address,sizeof(address)) < 0){
		perror("Couldn't bind to port");
		return -1;
	}
	
	printf("Listening\n");
	if(listen(es_socket,3) < 0){
		perror("Failed to listen");
		return -1;
	}
	int sd;
	int activity;
	addrlen = sizeof(address);
	printf("Waiting for connections ...\n");
	while(1){
		FD_ZERO(&rfds);
		FD_SET(es_socket,&rfds);
		max_sd = es_socket;
		for(int i = 0; i < max_clients;i++){
			sd = client_socket[i];
			if(sd > 0)
				FD_SET(sd,&rfds);
			if(sd > max_sd)
				max_sd = sd;
		}
		activity = select(max_sd + 1, &rfds, NULL, NULL,NULL);
		if(activity < 0){
			printf("Socket error");
		}
		if(FD_ISSET(es_socket,&rfds)){
			new_socket = accept(es_socket,(struct sockaddr *)&address,(socklen_t *)&addrlen);
			if(new_socket < 0){
				printf("Error accepting\n");
				return -1;
			}
			printf("Got connection!\n");
			int *command = malloc(80);
			send(new_socket,"Waiting for data\n",17,0);
			send(new_socket,&end,1,0);
			int c = 0;
			while(1){
				recv(new_socket,&c,1,0);
				if(c == end)
					break;
				printf("Got byte %#2x\n",c);
			}
			printf("Recieving Info from Client\n");
			printf("Waiting for initial character\n");
			c = 0;
			while(1){
				recv(new_socket,&c,1,0);
				if(c == end)
					break;
				printf("Did not recieve byte sleeping 1 second\n");
				sleep(1);
			}
			printf("Got byte!\n");
			printf("Recieving info\n");
			int *dbuf = malloc(1024);
			c = 0;
			int size = 0;
			while(1){
				recv(new_socket,&c,1,0);
				if(c == end)
					break;
				printf("Got byte:%#02x\n",c);
				dbuf[size] = c;
				size++;
			}
			struct client_user_dat *dat = parse_user_raw(dbuf);
			printf("Checking to see if name %s is avaliable\n",dat->user);
			DIR *d = opendir(cfg->user_path);
			struct dirent *ent;
			int i = 0;
			char *path = malloc(1024);
			for(i = 0; i < strlen(cfg->user_path);i++)
				path[i] = cfg->user_path[i];
			path[i] = '/';
			i++;
			char cc1 = 'a';
			path[i] = cc1;
			int nametaken = 0;
			int err = 0;
			while((ent = readdir(d))){
				if(ent->d_type == DT_REG){
					FILE *tmpf = fopen(path,"rb");
					if(!tmpf){
						int errsv = errno;
						if(errsv == ENOENT)
							break;
						perror("I/O Error");
						return -1;
					}
					struct client_user_dat *tmpdat = parse_user_file(tmpf);
					if(strcmp(tmpdat->user,dat->user) == 0 && strcmp(tmpdat->pass,dat->pass) == 0){
						nametaken = 1;
						break;
					}
					else if(strcmp(tmpdat->user,dat->user) == 0 && strcmp(tmpdat->pass,dat->pass) != 0){
						nametaken = 1;
						printf("Invalid login for user:%s\n",dat->user);
						int errc = ERR_INVALIDLOGIN;
						send(new_socket,&errc,1,0);
						new_socket = 0;
						err = 1;
					}
					fclose(tmpf);
				}

			}
			if(err == 1)
				continue;
			if(nametaken == 0){
				printf("Creating entry for user:%s\n",dat->user);
				FILE *f;
				char cc = 'a';
				char *name = malloc(1024);
		 		i = 0;
				for(i = 0; i < strlen(cfg->user_path);i++)
					name[i] = cfg->user_path[i];
				name[i] = '/';
				printf("Searching for avaliable file name\n");
				i++;
				name[i] = cc;
				int n = i;
				while(1){
					if(!(f = fopen(name,"rb"))){
						int errsv = errno;
						if(errsv == ENOENT)
							break;
						perror("I/O Error");
						return -1;
					}else{
						if(cc <= 'z'){
							name[n] = cc;
							cc++;
						}
						else{
							n++;
							cc = 'a';
							name[n] = cc;
						}
					}
					fclose(f);
				}
				printf("Opening file: %s\n",name);
				FILE *out = fopen(name,"wb");
				if(!out){
					perror("Error opening file");
					return -1;
				}
				printf("Writing %d bytes\n",size);
				for(int i = 0; i < size;i++)
					fputc(dbuf[i],out);
				fclose(out);
				printf("Login successful for user: %s\n",dat->user);
				int succ = LOGIN_SUCC;
				send(new_socket,&succ,1,0);
			}
			else{
				printf("Login successful for user: %s\n",dat->user);
				int succ = LOGIN_SUCC;
				send(new_socket,&succ,1,0);
			}
			pid_t pid;
			pid = fork();
			if(pid == 0){
				while(1){
					int cmd = 0x7F;
					printf("Reciving command\n");
					while(1){
						if(cmd != 0x7F)
							break;
						recv(new_socket,&cmd,1,0);
					}
					int res = ERR_INVALIDCMD;
					if(cmd == CMD_LSU || cmd == CMD_WS || cmd == CMD_LS)
						res = VALID_CMD;
					send(new_socket,&res,1,0);
					if(cmd == ERR_INVALIDCMD)
						continue;
					//send(new_socket,&end,1,0);
					if(cmd == CMD_LSU){
						printf("Got command List Users\n");
						DIR *d = opendir(cfg->user_path);
						if(!d){
							perror("I/O Error");
							return -1;
						}
						struct dirent *ent;
						while((ent = readdir(d)) != 0){
							if(ent->d_type == DT_REG){
								char *tmppath = malloc(1024);
								sprintf(tmppath,"%s/%s",cfg->user_path,ent->d_name);
								FILE *f = fopen(tmppath,"rb");
								if(!f){
									perror("Couldn't open file");
									return -1;
								}
								struct client_user_dat *d = parse_user_file(f);
								for(int i = 0; i < strlen(d->user);i++)
									send(new_socket,&d->user[i],1,0);
								int nl = '\n';
								send(new_socket,&nl,1,0);
								fclose(f);
							}
						}
						send(new_socket,&end,1,0);
					}
					else if(cmd == CMD_WS){
						printf("Got command write story\n");
						char *raw = malloc(1024);
						int c;
						int n = 0;
						while(1){
							recv(new_socket,&c,1,0);
							if(c == END)
								break;
							raw[n] = c;
							n++;
						}
						char *path = malloc(1024);
						int i = 0;
						for(i = 0; i < strlen(cfg->story_path);i++)
							path[i] = cfg->story_path[i];
						path[i] = '/';
						int cc = 'a';
						i++;
						path[i] = cc;
						while(1){
							FILE *tmpf = fopen(path,"rb");
							if(!tmpf){
								int errsv = errno;
								if(errsv == ENOENT)
									break;
								perror("I/O Error");
								return -1;
							}
							fclose(tmpf);
							if(cc < 'z'){
								cc++;
								path[i] = cc;
							}
							else{
								path[i] = cc;
								cc = 'a';
								i++;
							}
						}
						printf("Opening file %s\n",path);
						FILE *nf = fopen(path,"wb");
						if(!nf){
							perror("Couldnt open file");
							return -1;
						}
						struct txt_data *txt = malloc(sizeof(*txt) * sizeof(struct txt_data *));
						txt->alloc = 1;
						txt->data_length = 8 + strlen(raw) + strlen(dat->user);
						txt->txt_length = strlen(raw);
						txt->username_len = strlen(dat->user);
						strcat(txt->username,dat->user);
						strcat(txt->txt,raw);
						write_txt_data_file(nf,txt);
						send(new_socket,&end,1,0);
						fclose(nf);
					}else if(cmd == CMD_LS){
						printf("Got command list story\n");
						DIR *d = opendir(cfg->story_path);
						if(!d){
							perror("Couldn't open directory");
							return -1;
						}
						struct dirent *ent;
						while((ent = readdir(d)) != 0){
							if(ent->d_type == DT_REG){
								char *path = malloc(1024);
								sprintf(path,"%s/%s",cfg->story_path,ent->d_name);
								FILE *tmpfile = fopen(path,"rb");
								if(!tmpfile){
									perror("Couldn't open file");
									return -1;
								}
								printf("Opening file:%s\n",path);
								struct txt_data *tdat = parse_txt_data_file(tmpfile);
								fclose(tmpfile);
								printf("Sending user:%s\n",tdat->username);
								int tc = '[';
								send(new_socket,&tc,1,0);
								for(int i = 0; i < tdat->username_len;i++)
									send(new_socket,&tdat->username[i],1,0);
								tc = ']';
								send(new_socket,&tc,1,0);
								printf("Sending data:%s\n",tdat->txt);
								for(int i = 0; i < tdat->txt_length;i++)
									send(new_socket,&tdat->txt[i],1,0);
								int nl = '\n';
								send(new_socket,&nl,1,0);
							}
						}
						send(new_socket,&end,1,0);
					}

				}
			}
			printf("Done\n");
			i = 0;
			while(client_socket[i] != 0)
				i++;
			client_socket[i] = new_socket;
		}
		else{
			for(int i = 0; i < max_clients;i++){
				sd = client_socket[i];
				if(FD_ISSET(sd,&rfds)){
					int valread;
					if((valread = read(sd,buffer,1024)) == 0){
						close(sd);
						client_socket[i] = 0;
					}
				}
			}
		}
	}
	return 0;
}
