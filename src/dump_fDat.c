#include <stdio.h>
#include <stdlib.h>
#include <opensm_libio.h>
int main(int argc,char *argv[]){
	misc_devw();
	if(argc != 2)
		return -1;
	FILE *f = fopen(argv[1],"r+b");
	if(!(f))
		return -1;
	struct file_data *fdat = parse_file_data_file(f);
	printf("[type:int name:alloc]%d [TYPE:uint32_t name:data_length]%d [type:int name:path_len]%d [type:uint32_t name:file_size]%d [type: int name:username_len] %d [type:char [80] name:username] %s[type:char [1024] name:path]%s\n",fdat->alloc,fdat->data_length,fdat->path_len,fdat->file_size,fdat->username_len,fdat->username,fdat->path);
	return 0;
}
