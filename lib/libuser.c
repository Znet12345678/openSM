#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
#include <libuser.h>
struct client_user_dat *parse_user_file(FILE *in){
	if(!(in))
		return (struct client_user_dat *)-1;
	struct client_user_dat *ret = malloc(sizeof(struct client_user_dat *) * sizeof(*ret));
	ret->size = getc(in);
	ret->userlen = getc(in);
	ret->passlen = getc(in);
	for(int i = 0; i < ret->userlen; i++)
		ret->user[i] = getc(in);
	for(int i = 0; i < ret->passlen; i++)
		ret->pass[i] = getc(in);
	if(!ret->size || ret->userlen < 0 || ret->passlen < 0)
		return (struct client_user_dat *)-1;
	return ret;
}
int write_user_file(FILE *out,struct client_user_dat *d){
	if(!(out))
		return -1;
	putc(d->size,out);
	putc(d->userlen,out);
	putc(d->passlen,out);
	for(int i = 0; i < d->userlen;i++)
		putc(d->user[i],out);
	for(int i =0 ;i < d->passlen;i++)
		putc(d->pass[i],out);
	return 1;
}
struct client_user_dat *parse_user_raw(int *buf){
	struct client_user_dat *ret = malloc(sizeof(*ret) * sizeof(struct client_user_dat *));
	ret->size = buf[0];
	ret->userlen = buf[1];
	ret->passlen = buf[2];
	int i;
	for(i = 0; i < ret->userlen;i++)
		ret->user[i] = buf[3 + i];
	for(int j = 0; j < ret->passlen;j++)
		ret->pass[j] = buf[3 + i + j];
	return ret;
}
