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

int ines_fprintf(FILE *f);
u_int8_t ines_get_chr_count();
int ines_load(FILE *f);
int ines_store(FILE *f);
u_int8_t ines_get_mapper_number();
u_int8_t ines_get_prg_count();
void ines_set_prg_count(u_int8_t x);
void ines_unload();

#endif /* INES_H_ */
