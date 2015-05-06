/*
 * script_decoding_cfg.h
 *
 *  Created on: 15 nov. 2011
 *      Author: rid
 */

#ifndef SCRIPT_DECODING_CFG_H_
#define SCRIPT_DECODING_CFG_H_

#define DEFAULT_CPU_MEM_VALUE	(NULL)
#define DEFAULT_TABLE_VALUE		(NULL)
#define DEFAULT_ROM_VALUE		(NULL)
#define DEFAULT_XML_VALUE		(NULL)

typedef struct _scr_dec_cfg {
	const char *cpu_mem;
	const char *table;
	const char *rom;
	const char *xml;
} scr_dec_cfg_t;

scr_dec_cfg_t *scr_dec_cfg_init(int argc, char **argv);

void scr_dec_cfg_uinit(scr_dec_cfg_t *cfg);

#endif /* SCRIPT_DECODING_CFG_H_ */
