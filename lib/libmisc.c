#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libmisc.h>
#include <opensm_libio.h>
#define SEP_BEF 0
#define SEP_AFT 1
void misc_devw(){
	printf("Development tool\nDo not use if you don't know what you're doing(The data this tool provides will be useless to you)\n");
}
int __ccont(char *str,int c){
	while(*str != 0){
		if(*str == c)
			return 1;
		*str++;
	}
	return 0;
}
int __ncont(char *str,int c){
	int ret = 0;
	while(*str != 0){
		if(*str == c)
			ret++;
		*str++;
	}
	return ret;
}
char *__misc_sep(char *line,int n){
	if(__ncont(line,':') != 1)
		return (char *)-1;
	if(n == SEP_BEF){
		char *ret = malloc(1024);
		int i = 0;
		while(*line != ':' && *line != 0){
			ret[i] = *line;
			i++;
			*line++;
		}
		return ret;
	}
	else if(n == SEP_AFT){
		char *ret = malloc(1024);
		while(*line != ':' && *line != 0)
			*line++;
		*line++;
		while(*line == ' ')
			*line++;
		int i = 0;
		while(*line != 0){
			ret[i] = *line;
			i++;
			*line++;
		}
		return ret;
	}
	else{
		printf("[__misc_sep]Invalid arg\n");
		return (char *)-1;
	}
}
int isnum(char *str){
	while(*str != 0){
		if(*str < '0' || *str > '9')
			return 0;
		*str++;
	}
	return 1;
}
struct __server_cfg *parse_cfg(FILE *f){
	struct __server_cfg *ret = malloc(sizeof(struct __server_cfg *) * sizeof(*ret));
	char *line = malloc(1024);
	int b[5];
	int linenum = 1;
	while((fgets(line,1024,f))){
		line[strlen(line) - 1] = 0;
		char *args[2] = {__misc_sep(line,SEP_BEF),__misc_sep(line,SEP_AFT)};
		if(strcmp(args[0],"maxConnections") == 0){
			if(isnum(args[1]) == 0){
				printf("Invalid data type: %s\n@linenum %d line %s\n",args[1],linenum,line);
				return (struct __server_cfg *)-1;
			}
			ret->maxconnections = atoi(args[1]);
			b[0] = 1;
		}
		else if(strcmp(args[0],"maxUsers") == 0){
			if(isnum(args[1]) == 0){
				printf("Invalid data type: %s\n@linenum %d line %s\n",args[1],linenum,line);
				return (struct __server_cfg *)-1;
			}
			ret->maxusers = atoi(args[1]);
			b[1] = 1;
		}
		else if(strcmp(args[0],"user_path") == 0){
			strcpy(ret->user_path,args[1]);
			b[2] = 1;
		}
		else if(strcmp(args[0],"story_path") == 0){
			strcpy(ret->story_path,args[1]);
			b[3] = 1;
		}
		else if(strcmp(args[0],"file_path") == 0){
			strcpy(ret->file_path,args[1]);
			b[4] = 1;
		}
		else{
			printf("Invalid paremeter: %s\n@linenum %d line %s\n",args[0],linenum,line);
			return (struct __server_cfg *)-1;
		}
		linenum++;
	}
	if(b[0] != 1){
		printf("Couldn't find required parameter 'maxConnections'\n");
		return (struct __server_cfg *)-1;
	}
	if(b[1] != 1){
		printf("Couldn't find required parameter 'maxUsers'\n");
		return (struct __server_cfg *)-1;
	}
	if(b[2] != 1){
		printf("Couldn't find required parameter 'user_path'\n");
		return (struct __server_cfg *)-1;
	}
	if(b[3] != 1){
		printf("Couldn't find required parameter 'story_path'\n");
		return (struct __server_cfg *)-1;
	}
	if(b[4] != 1){
		printf("Couldn't find required parameter 'file_path'\n");
		return (struct __server_cfg *)-1;
	}
	return ret;
}
