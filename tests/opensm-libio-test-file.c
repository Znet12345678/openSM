#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
int __cmp_file_data(struct file_data *d1,struct file_data *d2,int verbose){
	unsigned const char *buf1 = (char *)d1;
	unsigned const char *buf2 = (char *)d2;
	int i = 0;
	while(i < d1->data_length){
		if(verbose == 1)
			printf("Comparing:[%#02x] == [%#02x]\n",buf1[i],buf2[i]);
		if(buf1[i] != buf2[i]){
			printf("Error:[%#03x] != [%#03x]",buf1[i],buf2[i]);
			return -1;
		}
		i++;
	}
	return 1;
}
int hasArg(int argc,char **argv,char *cmp){
	for(int i = 0; i < argc;i++)
		if(strcmp(cmp,argv[i]) == 0)
			return 1;
	return 0;
}
int main(int argc,char *argv[]){

	int verbose = 0;
	if(argc > 1)
		if(hasArg(argc,argv,"-v"))
			verbose = 1;
	struct file_data *orig = malloc(sizeof(struct file_data *) * sizeof(*orig));
	orig->alloc = 1;
	orig->data_length = 20;
	orig->path_len = 4;
	orig->username_len = 4;
	strcpy(orig->username,"test");
	strcpy(orig->path,"test");
	if(verbose == 1)
		printf("Writing to file\n");
	FILE *out = fopen("tmp","wb");
	if(!out){
		perror("Error opening file");
		return -1;
	}
	write_file_data_file(out,orig);
	if(verbose == 1)
		printf("Closing output file\n");
	fclose(out);
	if(verbose == 1)
		printf("Opening input file\n");
	FILE *in = fopen("tmp","rb");
	if(!in){
		printf("Error opening file\n");
		return -1;
	}
	if(verbose == 1)
		printf("Parsing data\n");
	struct file_data *res = parse_file_data_file(in);
	if(verbose == 1)
		printf("Comparing data\n");
	__cmp_file_data(orig,res,verbose);
	if(verbose == 1)
		printf("Cleaning up\n");
	remove("tmp");
	printf("Test succeded!\n");
	free(res);
	free(orig);
	return 0;
}
