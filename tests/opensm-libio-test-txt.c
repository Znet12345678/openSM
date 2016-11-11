#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
int __txt_cmp(struct txt_data *cmp1,struct txt_data *cmp2){
	if(cmp1->alloc != cmp2->alloc || cmp1->data_length != cmp2->data_length || cmp1->txt_length != cmp2->txt_length || cmp1->username_len != cmp2->username_len || strncmp(cmp1->username,cmp2->username,cmp1->username_len) != 0 || strncmp(cmp1->txt,cmp2->txt,cmp1->txt_length) != 0)
		return -1;
	return 0;
}
int main(int arg,char *argv[]){
	FILE *out = fopen("tmp","wb");
	if(!out){
		printf("Test failed due to file I/O error(Check your permissions)\n");
		return -1;
	}
	struct txt_data *data = malloc(sizeof(struct txt_data *) * sizeof(*data));
	data->alloc = 1;
	data->data_length = 22;
	data->txt_length = 4;
	strcpy(data->txt,"test");
	data->username_len = 4;
	strcpy(data->username,"null");
	write_txt_data_file(out,data);
	fclose(out);
	FILE *in = fopen("tmp","rb");
	if(!in){
		printf("Test failed due to file I/O error(Check your permissions)\n");
		return -1;
	}
	struct txt_data *read = malloc(sizeof(struct txt_data *) * sizeof(*read));
	read = parse_txt_data_file(in);
	if(__txt_cmp(read,data) == -1){
		printf("Test failed!\n");
		return -1;
	}
	free(data);
	remove("tmp");
	printf("Test succeded!\n");
	return 0;
}
