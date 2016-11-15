#ifndef __OPENSM_LIBCLIENT_H
#define __OPENSM_LIBCLIENT_H
/*
*Sends txt_data d to socket sockfd
*/
int send_txt_data(int sockfd,struct txt_data *d);
#endif
