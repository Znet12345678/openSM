/*
*(c) 2016 Zachary Schlotman
*/
#ifndef __OPENSM_LIBUSER_H
#define __OPENSM_LIBUSER_H
//Contains information on current user
struct client_user_dat{
	int size;
	int userlen;
	int passlen;
	char user[80];
	char pass[80];
};
/*
*Parses user file in(usually /etc/opensm/user.dat)
*Returns parsed file or -1 on error
*/
struct client_user_dat *parse_user_file(FILE *in);
/*
*Writes client_user_dat *d to output file out
*Returns 1 on success or <=0 on error
*/
int write_user_file(FILE *out,struct client_user_dat *d);
/*
*Parses user data from memmory
*shouldn't encounter any errors will Segfault if there is any issues with memory
*/
struct client_user_dat *parse_user_raw(int *buf);
#endif
