/*
*libIO for openSM
*Code written by:Zachary James Schlotman
*(c) 2016 Zachary James Schlotman
*/
#ifndef __OPENSM_LIBIO_H
#define __OPENSM_LIBIO_H
#include <libmisc.h>
#include <stdint.h>
#define END_BYTE 0x7f
/*
*Contains data on a message on a wall
*/
struct txt_data{
	int alloc;
	int data_length;
	int txt_length;
	int username_len;
	char username[80];
	char txt[1024];
};
/*
*Contains data on the location of a file posted to a wall
*/
struct file_data{
	int alloc;
	uint32_t data_length;
	int path_len;
	uint32_t file_size;
	int username_len;
	char username[80];
	char path[1024];
};

/*
*Contains data on the properties and contents of a file posted to a wall
*/
struct file_ent{
	int alloc;
	int entlen;
	int creatorLen;
	char creatorName[80];
	int namelen;
	char name[80];
	int datalen;
	//data
};
int write_file_ent_head(FILE *f,struct file_ent *ent);
struct file_ent *parse_file_ent_head(FILE *f);
/*
*Reads data from file f to memory then stores it in a struct txt_data
*Started:10/16/16
*Finished:
*Time:410
*Debug time:
*/
struct txt_data *parse_txt_data_file(FILE *f);
/*
*Reads data from buffer buf into struct txt_data
*Started:10/17/16
*Finished:10/17/16
*Time:59
*/
struct txt_data *parse_txt_data(const char *buf);
/*
*Reads data from file into file_data structure
*Started:10/17/16
*Finished:10/17/16
*Time:334
*/
struct file_data *parse_file_data_file(FILE *f);
/*
*Reads data from buf into file_data structure
*Started:
*Finished:
*Time:
*/
struct file_data *parse_file_data(const char *buf);
/*
*Writes data to file f from struct txt_data
*Started:10/16/16
*Finished:
*Time:179
*Debug time:
*/
int write_txt_data_file(FILE *f,struct txt_data *d);
/*
*Writes data from struct txt_data d to the terminal buffer
*Started:
*Finished:
*Time:
*/
int write_txt_data_stdout(struct txt_data *d);
/*
*Writes data from struct file_data d to file f
*Started:10/17/16
*Finished:
*Time:207
*/
int write_file_data_file(FILE *f,struct file_data *d);
/*
*
*
*
*
*/
int write_file_data_stdout(struct file_data *d);
/*
*
*
*
*
*/
int write_str_file(FILE *f,struct file_ent *fent,int *data);
/*
*
*
*
*
*/
int init_bin_file(FILE *f,struct file_ent *fent);
/*
*
*
*
*
*/
int write_bin_file(FILE *f,int data);
#endif
