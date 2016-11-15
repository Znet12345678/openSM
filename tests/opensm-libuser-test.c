#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
#include <libuser.h>
int comparestruct(struct client_user_dat *s1,struct client_user_dat *s2){
	if(s1->size != s2->size)
		return -1;
	const unsigned char *s1C = (const unsigned char *)s1;
	const unsigned char *s2C = (const unsigned char *)s2;
	for(int i = 0; i < s1->size;i++)
		if(s1C[i] != s2C[i])
			return -1;
	return 0;
}
int getarg(int argc, char *argv[],char *arg){
	for(int i = 0; i < argc;i++)
		if(strcmp(argv[i],arg) == 0)
			return 0;
	return -1;
}
int main(int argc, char *argv[]){
	if(getarg(argc,argv,"-v") == 0)
		printf("Starting test\n");
	struct client_user_dat *orig = malloc(sizeof(struct client_user_dat *) * sizeof(*orig));
	struct client_user_dat *fin = malloc(sizeof(struct client_user_dat *) * sizeof(*fin));
        if(getarg(argc,argv,"-v") == 0)
                printf("Setting array\n");
	orig->size = 11;
	orig->userlen = 4;
	orig->passlen = 4;
	strcpy(orig->user,"test");
	strcpy(orig->pass,"1234");
	if(getarg(argc,argv,"-v") == 0)
		printf("Opening output file\n");
	FILE *out = fopen("tmp","wb");
	if(!out){
		perror("Test failed due to file I/O error");
                return -1;
	}
        if(getarg(argc,argv,"-v") == 0)
                printf("Writing file\n");
	write_user_file(out,orig);
	if(getarg(argc,argv,"-v") == 0)
		printf("Closing file\n");
	fclose(out);
        if(getarg(argc,argv,"-v") == 0)
                printf("Opening input file\n");
	FILE *in = fopen("tmp","rb");
	if(!in){
		perror("Test failed due to file I/O error");
		return -1;
	}
        if(getarg(argc,argv,"-v") == 0)
                printf("Parsing file\n");
	fin = parse_user_file(in);
        if(getarg(argc,argv,"-v") == 0)
                printf("Comparing file\n");
	if(comparestruct(orig,fin) != 0){
		printf("Test failed!\n");
		return -1;
	}
	remove("tmp");
	printf("Test succeeded!\n");
}
