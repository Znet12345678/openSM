#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
#include <libclient.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	int es_sock,addrlen;
	int port = 2718;
	es_sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in *server_address = malloc(sizeof(struct sockaddr_in *) * sizeof(*server_address));
	server_address->sin_family = AF_INET;
	server_address->sin_port = htons(2718);
	server_address->sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(es_sock,(struct sockaddr *)server_address,sizeof(*server_address)) < 0){
		perror("Failed to connect");
		return -1;
	}
	int size;
	recv(es_sock,&size,1,0);
	printf("Recieving %d bytes\n",size);
	int *buf = malloc(1024);
	recv(es_sock,buf,size,0);
	for(int i = 0; i < size;i++)
		printf("Recieved byte:%d\n",buf[i]);
	close(es_sock);
	free(server_address);
}
