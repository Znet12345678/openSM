#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
	printf("Done\n");
	return 0;
}
