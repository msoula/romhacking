/*
 * command.h
 *
 *  Created on: 21 juin 2012
 *      Author: msoula
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdio.h>
#include <sys/types.h>

typedef struct _command {
	unsigned int offset;
	u_int8_t bytes[3];		 // 1, 2 or 3 bytes (see addressing mode)
	struct _command *prev;	 // previous command
	struct _command *next;	 // next command
} command_t;

command_t *command_init();
void command_destroy(command_t *cmd);

const char *command_to_str(command_t *cmd, u_int16_t addr_offset);

#endif /* COMMAND_H_ */
