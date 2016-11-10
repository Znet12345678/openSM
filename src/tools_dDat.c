#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <opensm_libio.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	FILE *f = fopen(argv[1],"rb");
	if(!(f))
		return -1;
	struct txt_data *d = parse_txt_data_file(f);
	printf("%s says %s\n",d->username,d->txt);
}
