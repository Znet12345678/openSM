#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
int main(int argc, char *argv[]){
	if(argc != 3)
		return -1;
	FILE *f = fopen(argv[1],"wb");
	if(!(f)){
		printf("Error opening %s\n",argv[1]);
		return -1;
	}
	struct file_data *data = malloc(sizeof(struct file_data *));
	data->alloc = 1;
	data->data_length = strlen(argv[2]) + 16;
	data->username_len = 4;
	strcpy(data->username,"null");
	data->path_len = strlen(argv[2]);
	FILE *gs = fopen(argv[2],"rb");
	if(!(gs)){
		printf("Error opening %s\n",argv[2]);
		return -1;
	}
	fseek(gs,0,SEEK_END);
	int filesize = ftell(gs);
	data->file_size = filesize;
	strcpy(data->path,argv[2]);
	int ret = write_file_data_file(f,data);
	fclose(gs);
	fclose(f);
	return ret;
}
