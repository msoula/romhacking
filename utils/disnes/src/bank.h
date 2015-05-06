/*
 * bank.h
 *
 *  Created on: 21 juin 2012
 *      Author: msoula
 */

#ifndef BANK_H_
#define BANK_H_

#include "command.h"
#include "opcodes.h"

typedef struct _bank {
	command_t *first;
	command_t *last;

	u_int16_t start_address;

	u_int16_t *jsr_offsets;
	unsigned short jsr_count;
	unsigned short jsr_size;

	size_t size;
} bank_t;

bank_t *bank_init();
void bank_destroy(bank_t *bank);

int bank_append(bank_t *bank, command_t *cmd);
int bank_load(bank_t *bank, void *data, size_t size);

void bank_set_start_address(bank_t *bank, u_int16_t start_addr);

void bank_fprintf(FILE *f, bank_t *bank);

#endif /* BANK_H_ */
