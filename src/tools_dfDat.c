#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	FILE *f = fopen(argv[1],"rb");
	struct file_data *df = parse_file_data_file(f);
	if(!(df))
		return -1;
	printf("%s says %s\n",df->username,df->path);
	return 0;
}
