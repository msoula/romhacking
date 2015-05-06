/*
 * disnes_cfg.h
 *
 *  Created on: 4 juillet 2012
 *      Author: rid
 */

#ifndef DISNES_CFG_H_
#define DISNES_CFG_H_

#define BANK_LIST_SIZE			32
#define DEFAULT_BANK_SIZE		0x4000
#define DEFAULT_BANK_ADDR		0x8000
#define DEFAULT_NO_BRANCHES		0

#include <sys/types.h>

typedef struct _disnes_cfg {
	int banks[BANK_LIST_SIZE];	/* list of prg-rom bank number to dis */
	int bank_size;				/* size of one bank in bytes */
	u_int16_t bank_addr;		/* output bank starting address*/
	int no_branches;			/* 0 to print branches, 1 otherwise */
	char *rom;					/* rom filename */
} disnes_cfg_t;

disnes_cfg_t *disnes_cfg_init(int argc, char **argv);

void disnes_cfg_uinit(disnes_cfg_t *cfg);

#endif /* DISNES_CFG_H_ */
