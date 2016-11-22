/*
*Libio c file
*Written by Zachary James Schlotman
*(c) 2016 Zachary James Schlotman
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
struct txt_data *parse_txt_data_file(FILE *f){
	struct txt_data *ret = malloc(sizeof(struct txt_data *) * sizeof(*ret));
	/*fseek(f,1,SEEK_SET);
	int size = getc(f) << 24 | getc(f) << 16 | getc(f) << 8 | getc(f);
	int *buf = malloc(size);
	fseek(f,0,SEEK_SET);
	int i = 0;
	int c;
	while((c = getc(f)) != EOF){
		buf[i] = c;
		i++;
	}
	ret->alloc = buf[0];
	ret->data_length = buf[1] << 24 | buf[2] << 16 | buf[3] << 8 | buf[4];
	ret->txt_length = buf[5] << 8 | buf[6];
	ret->username_len = buf[7];
	i = 0;
	int j = 0;
	for(j = 0;j < ret->username_len;j++)
		ret->username[j] = buf[j + 8];
	for(i = 0; i < ret->txt_length;i++)
		ret->txt[i] = buf[i + 8 + j];*/
	ret->alloc = getc(f);
	ret->data_length = getc(f) << 24 | getc(f) << 16 | getc(f) << 8 | getc(f);
	ret->txt_length = getc(f) << 8 | getc(f);
	ret->username_len = getc(f);
	for(int i = 0; i < ret->username_len;i++)
		ret->username[i] = getc(f);
	for(int i = 0; i < ret->txt_length;i++)
		ret->txt[i] = getc(f);

	return ret;
}
int write_txt_data_file(FILE *f,struct txt_data *d){
	if(!(f))
		return -1;
	if(!(d))
		return -1;
	putc(d->alloc,f);
	putc(d->data_length >> 24,f);
	putc(d->data_length >> 16,f);
	putc(d->data_length >> 8,f);
	putc(d->data_length,f);
	putc(d->txt_length >> 8,f);
	putc(d->txt_length,f);
	putc(d->username_len,f);
	for(int i = 0; i < d->username_len;i++)
		putc(d->username[i],f);
	for(int i = 0; i < d->txt_length;i++)
		putc(d->txt[i],f);

	return 1;
}
struct txt_data *parse_txt_data(const char *buf){
	struct txt_data *ret = malloc(sizeof(struct txt_data *) * sizeof(*ret));
	ret->alloc = buf[0];
        ret->data_length = buf[1] << 24 | buf[2] << 16 | buf[3] << 8 | buf[4];
        ret->txt_length = buf[5] << 8 | buf[6];
	ret->username_len = buf[7];
	int j;
	for(j = 0;j < ret->username_len;j++)
		ret->username[j] = buf[j + 8];
	int i = 0;
	for(i = 0; i < ret->txt_length;i++)
                ret->txt[i] = buf[i + 8 + j];

        return ret;

}
struct file_data *parse_file_data_file(FILE *f){
	struct file_data *ret = malloc(sizeof(struct file_data *) * sizeof(*ret));
	int c;
	ret->alloc = fgetc(f);
	ret->data_length = fgetc(f) << 24 | fgetc(f) << 16 | fgetc(f) << 8| fgetc(f);
	ret->path_len = fgetc(f) << 8 | fgetc(f);
	ret->file_size = fgetc(f) << 24 | fgetc(f) << 16 | fgetc(f) << 8 | fgetc(f);
	ret->username_len = fgetc(f);
	int j = 0;
	for(j = 0;j < ret->username_len;j++)
		ret->username[j] = fgetc(f);
	ret->username[j] = 0;
	for(int i = 0; i < ret->path_len;i++)
		ret->path[i] = fgetc(f);
	return ret;
}
int write_file_data_file(FILE *f,struct file_data *d){
	if(!(f))
		return -1;
	putc(d->alloc,f);
	putc(d->data_length >> 24,f);
	putc(d->data_length >> 16,f);
	putc(d->data_length >> 8,f);
	putc(d->data_length,f);
	putc(d->path_len >> 8,f);
	putc(d->path_len,f);
	putc(d->file_size >> 24,f);
	putc(d->file_size >> 16,f);
	putc(d->file_size >> 8,f);
	putc(d->file_size,f);
	putc(d->username_len,f);
	for(int i= 0; i < d->username_len;i++)
		putc(d->username[i],f);
	for(int i = 0; i < d->path_len;i++)
		putc(d->path[i],f);
	return 1;
}
int write_file_ent_head(FILE *f,struct file_ent *ent){
	putc(ent->alloc,f);
	putc(ent->entlen,f);
	putc(ent->creatorLen,f);
	for(int i = 0; i < ent->creatorLen; i++)
		putc(ent->creatorName[i],f);
	putc(ent->namelen,f);
	for(int i = 0; i < ent->namelen;i++)
		putc(ent->name[i],f);
	putc(ent->datalen >> 24,f);
	putc(ent->datalen >> 16,f);
	putc(ent->datalen >> 8,f);
	putc(ent->datalen,f);
	return 1;
}
struct file_ent *parse_file_ent_head(FILE *f){
	struct file_ent *ret = malloc(sizeof(*ret) * sizeof(struct file_ent *));
	ret->alloc = getc(f);
	ret->entlen = getc(f);
	ret->creatorLen = getc(f);
	for(int i = 0; i < ret->creatorLen;i++)
		ret->creatorName[i] = getc(f);
	ret->namelen = getc(f);
	for(int i = 0; i < ret->namelen;i++)
		ret->name[i] = getc(f);
	ret->datalen = getc(f) << 24 | getc(f) << 16 | getc(f) << 8 | getc(f);
	return ret;
}
