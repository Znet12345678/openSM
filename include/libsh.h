#ifndef __OPENSM_LIBSH_H
#define __OPENSM_LIBSH_H
int parse_command(char *cmd);
int __sep(char *ret[],char *str,char c);
int countc(char *str,char c);
#endif
