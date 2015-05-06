/*
 * command.c
 *
 *  Created on: 21 juin 2012
 *      Author: msoula
 */

#include "command.h"

#include "opcodes.h"

#include <stdlib.h>
#include <string.h>

#define STR_SIZE	(1024)
static char str[STR_SIZE] = {0};

command_t *command_init() {
	command_t *cmd;
	if(NULL == (cmd = (command_t *) malloc(sizeof(command_t))))
		return NULL;

	cmd->offset = 0;
	cmd->prev = cmd->next = NULL;

	return cmd;
}

void command_destroy(command_t *cmd) {
	if(cmd)
		free(cmd);
}

const char *command_to_str(command_t *cmd, u_int16_t addr_offset) {

	int length;
	memset(str, '\0', STR_SIZE);

	length = snprintf(str, STR_SIZE, "$%04X    ", cmd->offset + addr_offset);
	switch(instruction_param_number(instruction_set[cmd->bytes[0]])) {
		case 0:
			length += snprintf(&str[length], STR_SIZE - length, "      %02X    ", cmd->bytes[0]);
			length += snprintf(&str[length], STR_SIZE - length, "%s ", instruction_to_str(instruction_set[cmd->bytes[0]]));
			length += snprintf(&str[length], STR_SIZE - length, instruction_param_format(instruction_set[cmd->bytes[0]]));
			break;
		case 1:
			length += snprintf(&str[length], STR_SIZE - length, "   %02X %02X    ", cmd->bytes[0], cmd->bytes[1]);
			length += snprintf(&str[length], STR_SIZE - length, "%s ", instruction_to_str(instruction_set[cmd->bytes[0]]));
			length += snprintf(&str[length], STR_SIZE - length, instruction_param_format(instruction_set[cmd->bytes[0]]), cmd->bytes[1]);
			break;
		case 2:
			length += snprintf(&str[length], STR_SIZE - length, "%02X %02X %02X    ", cmd->bytes[0], cmd->bytes[1], cmd->bytes[2]);
			length += snprintf(&str[length], STR_SIZE - length, "%s ", instruction_to_str(instruction_set[cmd->bytes[0]]));
			length += snprintf(&str[length], STR_SIZE - length, instruction_param_format(instruction_set[cmd->bytes[0]]), cmd->bytes[2], cmd->bytes[1]);
			break;
	}

	while(length < 35)
		str[length++] = ' ';
	str[length++] = ';';

	return str;
}
