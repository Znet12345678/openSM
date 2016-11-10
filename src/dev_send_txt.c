#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libclient.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	printf("Starting server\n");
	printf("Establishing socket\n");
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(!(sock))
		return -1;
	int port = 2718;
	printf("Binding port %d\n",port);
	struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in *));
	address->sin_family = AF_INET;
	address->sin_port = htons(port);
	address->sin_addr.s_addr = INADDR_ANY;
	if(bind(sock,(struct sockaddr *)address,sizeof(*address)) != 0)
		return -1;
	printf("Listening\n");
	if(listen(sock,1) < 0)
		return -1;
	printf("Now accepting connections\n");
	int s = sizeof(*address);
	int new_socket = accept(sock,(struct sockaddr *)address,&s);
	FILE *in = fopen(argv[1],"rb");
	if(!(in))
		return -1;
	struct txt_data *data = parse_txt_data_file(in);
	printf("Sending data w/ contents:\nalloc[type:int value:%d]\tdata_length[type:int value:%d]\ttxt_length[type:int value:%d]\tusername_len[type:int value:%d]\tusername[type:char*  value:%s]\ttxt[type:char* value:%s]\t\n",data->alloc,data->data_length,data->txt_length,data->username_len,data->username,data->txt);
	send_txt_data(new_socket,data);
	free(address);
	free(data);
	close(new_socket);
	close(sock);

	return 0;
}
