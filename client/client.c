#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opensm_libio.h>
#include <libuser.h>
#include <libmisc.h>
#include <errno.h>
#include <libsh.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	int sock;
	sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in in;
	in.sin_family = AF_INET;
	in.sin_port = htons(2718);
	in.sin_addr.s_addr = inet_addr(argv[1]);
	printf("Connecting...\n");
	if(connect(sock,(struct sockaddr *)&in,sizeof(in)) < 0){
		perror("Failed to connect");
		return -1;
	}
	printf("Reciving initial string\n");
	int cmd[10] = {[0 ... 9]0};
	char c;
	while(c != 0x7F){
		recv(sock,&c,1,0);
		if(c == 0x7F)
			break;
		printf("Got byte: %c\n",c);
	}
	printf("Sending command\n");
	for(int i = 0; i < 10;i++)
		send(sock,&cmd[i],1,0);
	int end = END;
	send(sock,&end,1,0);
	int err = mkdir("/usr/local/share/opensm",0777);
	FILE *f = fopen("/usr/local/share/opensm/user.dat","rb");
	if(!(f)){
		char *user = malloc(1024);
		char *pass = malloc(1024);
		FILE *userout = fopen("/usr/local/share/opensm/user.dat","wb");
                if(!userout){
			perror("I/O error on /usr/local/share/opensm/user.dat");
                        return -1;
                }
		while(1){
			printf("Enter Username (one word)>>>");
			scanf("%s",user);
			printf("Enter Password (one word)>>>");
			scanf("%s",pass);
			printf("This data will be storred unencrypted!Write to file?(Y/N)");
			char *opt = malloc(80);
			scanf("%s",opt);
			if(strcasecmp(opt,"y") == 0 || strcasecmp(opt,"yes") == 0)
				break;
		}
		struct client_user_dat *dat = malloc(sizeof(struct client_user_dat *) * sizeof(*dat));
		dat->size = strlen(user) + strlen(pass) + 3;
		dat->userlen = strlen(user);
		dat->passlen = strlen(pass);
		strcpy(dat->user,user);
		strcpy(dat->pass,pass);
		if(write_user_file(userout,dat) < 0){
			printf("Error\n");
			return -1;
		}
		fclose(userout);
	}
	FILE *inf;
	inf = fopen("/usr/local/share/opensm/user.dat","rb");
	struct client_user_dat *fin = malloc(sizeof(struct client_user_dat *) * sizeof(*fin));
	fin = parse_user_file(inf);
	printf("Sending end byte\n");
	send(sock,&end,1,0);
	printf("Sending data\n");
	send(sock,&fin->size,1,0);
	send(sock,&fin->userlen,1,0);
	send(sock,&fin->passlen,1,0);
	for(int i = 0; i < fin->userlen;i++)
		send(sock,&fin->user[i],1,0);
	for(int i = 0; i < fin->passlen;i++)
		send(sock,&fin->pass[i],1,0);
	send(sock,&end,1,0);
	int loginret = 0;
	recv(sock,&loginret,1,0);
	if(loginret == ERR_INVALIDLOGIN){
		printf("Invalid login\n");
		return -1;
	}
	else if(loginret == LOGIN_SUCC){
		printf("Successfully logged in\n");

	}
	while(1){
		char *cmd = malloc(1024);
		printf(">");
		fgets(cmd,1024,stdin);
		cmd[strlen(cmd) - 1] = 0;
		printf("Processing command: %s\n",cmd);
		int cmds = parse_command(cmd);
		if(cmds == CMD_EXIT)
			break;
		else if(cmds == CMD_LSU || cmds == CMD_LS)
			send(sock,&cmds,1,0);
		else if(cmds == CMD_WS){
			//if(countc(cmd,' ') != 1){
			//	printf("Invalid number of arguments\n");
			//	continue;
			//}
			send(sock,&cmds,1,0);
			char *args[countc(cmd,' ') + 1];
			__sep(args,cmd,' ');
			for(int i = 1; i < countc(cmd,' ') + 1;i++){
				for(int j = 0; j < strlen(args[1]);j++)
					send(sock,&args[i][j],1,0);
				send(sock," ",1,0);
			}
			send(sock,&end,1,0);
		}
		else if(cmds == CMD_WRITE){
			int zero = 0;
			send(sock,&cmds,1,0);

			int sb = 0x0f;
			int byte = 1;

			if(countc(cmd,' ') != 1){
				printf("Invalid number of arguments\n");
				continue;
			}
			char *args[countc(cmd,' ') + 1];
			__sep(args,cmd,' ');
			FILE *f = fopen(args[1],"rb");
			if(!f){
				perror("Failed to open file for reading");
				continue;
			}
			a:printf("Enter name to save file under >");
			char *svname = malloc(1024);
			scanf("%s",svname);
			//fgets(svname,1024,stdin);
			int s = strlen(svname);
			send(sock,&s,1,0);
			printf("Using name:%s\n",svname);
			for(int i = 0; i < strlen(svname);i++){
				send(sock,&svname[i],1,0);
			}
			send(sock,&end,1,0);
			fseek(f,0,SEEK_END);
			uint32_t size = ftell(f);
			fseek(f,0,SEEK_SET);
			printf("Read file %d bytes\n",size);
			int res, _sendb = 0x1F;
			recv(sock,&res,1,0);
			while(1){
				if(res == 5)
					break;
				else if(res == 1){
					printf("Invalid name!\n");
					goto a;
				}
				else
					recv(sock,&res,1,0);
			}
			/*int res = 0;
			recv(sock,&res,1,0);
			if(res != 5)
				goto a;
			struct file_data *fdat = malloc(sizeof(*fdat) * sizeof(struct file_data *));
			fdat->alloc = 1;
			fdat->data_length = 12 + strlen(fin->user) + strlen(args[1]);
			fseek(f,0,SEEK_END);
			fdat->file_size = ftell(f);
			fseek(f,0,SEEK_SET);
			fdat->username_len = strlen(fin->user);
			fdat->path_len = strlen(args[1]);
			strcat(fdat->username,fin->user);
			strcat(fdat->path,args[1]);
			char bytes[] = {fdat->alloc,fdat->data_length >> 24, fdat->data_length >> 16,fdat->data_length >> 8,fdat->data_length,fdat->path_len >> 8,fdat->path_len,fdat->file_size >> 24,fdat->file_size >> 16,fdat->file_size >> 8,fdat->file_size,fdat->username_len};
			for(int i = 0; i < 12;i++)
				send(sock,&bytes[i],1,0);
			for(int i = 0; i < fdat->username_len; i++)
				send(sock,&fdat->username[i],1,0);
			for(int i = 0; i < fdat->path_len;i++)
				send(sock,&fdat->path[i],1,0);
			send(sock,&end,1,0);*/
			struct file_ent *fent = malloc(sizeof(*fent) * sizeof(struct file_ent *));
			fent->alloc = 1;
			fent->entlen = 7 + strlen(fin->user);
			fent->creatorLen = strlen(fin->user);
			strcat(fent->creatorName,fin->user);
			fent->namelen = strlen(svname);
			strcat(fent->name,svname);
			fent->datalen = size;
			char _bytes[] = {fent->alloc,fent->entlen,fent->creatorLen,fent->datalen >> 24,fent->datalen >> 16,fent->datalen >> 8,fent->datalen};
			for(int i = 0; i < 7; i++)
				send(sock,&_bytes[i],1,0);
			for(int i = 0; i < fent->creatorLen;i++)
				send(sock,&fent->creatorName[i],1,0);
			send(sock,&end,1,0);
			//printf("Uploading file\n");
			uint32_t nsize = htonl(size);
			//char by[4] = {size >> 24,size >> 16,size >> 8,size};
			send(sock,&nsize,sizeof(uint32_t),0);
			//for(int i = 0; i < 4;i++)
			//	printf("[%d]\n",by[i]);
			printf("Uploading file\n");
			int bytes_written = 0;
			int c = 0;
			while((c = getc(f)) != EOF){
				send(sock,&c,1,0);
				bytes_written++;
			}
			fclose(f);
		}else if(cmds == CMD_LF){

		}
		else{
			printf("Invalid Command!\n");
			continue;
		}
		int res = 0;
		recv(sock,&res,1,0);
		//send(sock,&end,1,0);
		if(res != VALID_CMD){
			printf("An error occured sending the command to the server\n");
			continue;
		}
		int c = 0;
		if(cmds != CMD_WRITE){
			while(1){
				recv(sock,&c,1,0);
				if(c == END)
					break;
				printf("%c",c);
			}
		}
		send(sock,&end,1,0);
	}
	printf("Done\n");
	return 0;
}
