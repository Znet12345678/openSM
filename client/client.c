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
#include <errno.h>
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
		perror("Failed to connect\n");
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
	int end = 0x7F;
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
	printf("Done\n");
	return 0;
}
