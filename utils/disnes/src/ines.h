/*
 * ines.h
 *
 *  Created on: 19 juin 2012
 *      Author: rid
 */

#ifndef INES_H_
#define INES_H_

#include <stdio.h>
#include <sys/types.h>

typedef struct _fprintf_data {
	void (* header)(FILE *, void *);
	void *header_data;
	void (* footer)(FILE *, void *);
	void *footer_data;
} fprintf_data_t;

typedef struct _ines {
	char header[4];
	u_int8_t prg_rom;
	u_int8_t chr_rom;
	u_int8_t ctrl_byte_1;
	u_int8_t ctrl_byte_2;
} ines_t;

typedef struct _mapper {
	u_int8_t number;
	const char *name;
} mapper_t;

extern mapper_t mappers[];

int ines_fprintf(FILE *f, ines_t *ines, fprintf_data_t *data);
int ines_load(ines_t *ines, FILE *f);

#endif /* INES_H_ */
