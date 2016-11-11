/*
*openSM server prototype
*(c) 2016 Zachary James Schlotman
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
int main(int argc,char *argv[]){
	printf("openSM Server V0.1-Alpha\n");
	printf("Starting up\n");
	int opt = 1;
	int end = 0x7F;
	int es_socket,addrlen,new_socket,client_socket[200],max_clients = 200;
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
			int c;
			while(1){
				recv(new_socket,&c,1,0);
				if(c == end)
					break;
				printf("Got byte %#2x\n",c);
			}
			int i = 0;
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
