#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <opensm_libio.h>
#include <libclient.h>
#include <opensm_libsh.h>
int countc(char *str,char c){
	int ret = 0;
	for(int i = 0; i < strlen(str);i++)
		if(str[i] == c)
			ret++;
	return ret;
}
int __sep(char *ret[],char *str,char c){
	char *tmp = malloc(1024);
	ret[0] = malloc(80);
	int i = 0;
	int n = 0;
	for(int j = 0; j < strlen(str);j++){
		if(str[j] == c){
			i++;
			ret[i] = malloc(80);
			n = 0;
			continue;
		}
		ret[i][n] = str[j];
		n++;
	}
	return 1;
}
int parse_command(char *cmd){
	char *args[countc(cmd,' ') + 1];
	__sep(args,cmd,' ');
	if(strcmp(args[0],"lsusr") == 0)
		return CMD_LSU;
	else if(strcmp(args[0],"cstry") == 0)
		return CMD_CSTRY;
	else if(strcmp(args[0],"ws") == 0)
		return CMD_WS;
	else if(strcmp(args[0],"ls") == 0)
		return CMD_LS;
	else if(strcmp(args[0],"exit") == 0)
		return CMD_EXIT;
	else if(strcmp(args[0],"write") == 0)
		return CMD_WRITE;
	else
		return ERR_INVALIDCMD;
}
