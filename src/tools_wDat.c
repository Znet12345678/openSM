#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
int main(int argc, char *argv[]){
	if(argc != 3)
		return -1;
	printf("Arg dump:");
	for(int i = 0; i < argc;i++)
		printf("[%s]",argv[i]);
	printf("\n");
	FILE *f = fopen(argv[1],"wb");
	if(!(f))
		return -1;
	char *username = malloc(1024);
	username = "null";
	struct txt_data *dat = malloc(sizeof(struct txt_data *));
	dat->alloc = 1;
	dat->data_length = 11 + strlen(argv[2]) + strlen(username);
	dat->txt_length = strlen(argv[2]);
	dat->username_len = strlen(username);
	strcpy(dat->username,username);
	strcpy(dat->txt,argv[2]);
	write_txt_data_file(f,dat);
	fclose(f);
	free(dat);
	return 0;
}
