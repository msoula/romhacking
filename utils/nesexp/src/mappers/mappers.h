/*
 * mappers.h
 *
 *  Created on: 14 juil. 2012
 *      Author: rid
 */

#ifndef MAPPERS_H_
#define MAPPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../utils/logger.h"

#define CHR_ROM_BANK_SIZE	(8 * 1024)
#define PRG_ROM_BANK_SIZE	(16 * 1024)

/**
 * Fonction for expansion.
 * @param file:	rom to expand.
 * @return 0 if operation succeed, -1 otherwise.
 */
typedef int (*ExpandFunction)(FILE *in, u_int8_t prg_16kb_banks, u_int8_t chr_8kb_banks);

typedef enum _mapper_id {
	NROM,		/*!< 0x00. */
	MMC1,		/*!< 0x01. */
	MMC3,		/*!< 0x04. */
	MAPPER070,	/*!< 0x46. */
} mapper_id;

struct _mapper_desc{
	mapper_id id;			 /* the id declared in allfilters.h */
	short number;			 /* the iNes mapper number*/
	const char *name;		 /* filter name */
	ExpandFunction expand;	 /* called to rom expansion*/
};

/**
 * Structure for mapper's description.
 * @var mapper_desc
 */
typedef struct _mapper_desc mapper_desc_t;

void mapper_register(mapper_desc_t *desc);
void mapper_unregister_all();

mapper_desc_t *mapper_get_by_number(short number);

#endif /* MAPPERS_H_ */
