#ifndef __OPENSM_SH_H
#define __OPENSM_SH_H
#define CMD_LIST_USERS 0x00
#define CMD_LIST_STORIES 0x01
#define CMD_WRITE_STORY 0x02
int __cmd_list_users();
int __cmd_list_stories();
int __cmd_write_story(char *msg);
#endif
