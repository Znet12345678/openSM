#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <opensm_libio.h>
int send_txt_data(int sockfd,struct txt_data *d){
	int size = (12 + d->username_len + d->txt_length);
	send(sockfd,&size,1,0);
	send(sockfd,d,size,0);
	return 1;
}
