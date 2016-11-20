#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libmisc.h>
#include <opensm_libio.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	FILE *f = fopen(argv[1],"r");
	if(!f)
		return -1;
	struct __server_cfg *test = parse_cfg(f);
	if(test == (struct __server_cfg *)-1)
		return -1;
	printf("dump: %d\t%d\t%s\t%s\n",test->maxconnections,test->maxusers,test->user_path,test->story_path);
	return 0;
}
