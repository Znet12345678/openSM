#ifndef __OPENSM_LIBMISC_H
#define __OPENSM_LIBMISC_H
#include <opensm_libio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#define ERR_INVALIDLOGIN 0x1F
#define ERR_INVALIDCMD 0x3F
#define VALID_CMD 0x2F
#define LOGIN_SUCC 0x0F
#define CMD_LSU 0x00
#define CMD_CSTRY 0x01
#define CMD_WS 0x02
#define CMD_LS 0x03
#define CMD_EXIT 0x04
#define CMD_WRITE 0x05
#define CMD_LF 0x06
#define END 0x7F
#define END_SEC 0x6F
//prints Developer warning
void misc_devw();
struct __server_cfg{
	uint32_t maxconnections;
	uint32_t maxusers;
	char user_path[80];
	char story_path[80];
	char file_path[80];
};
struct __server_cfg *parse_cfg(FILE *f);
#endif
