/*
 * script_encoding_cfg.h
 *
 *  Created on: 15 nov. 2011
 *      Author: rid
 */

#ifndef SCRIPT_DECODING_CFG_H_
#define SCRIPT_DECODING_CFG_H_

#define DEFAULT_CPU_MEM_VALUE   (NULL)
#define DEFAULT_DICT_VALUE      (NULL)
#define DEFAULT_ROM_VALUE       (NULL)
#define DEFAULT_SCRIPT_VALUE    (NULL)
#define DEFAULT_TABLE_VALUE     (NULL)

typedef struct _scr_enc_cfg {
	const char *cpu_mem;
	const char *dict;
	const char *rom;
	const char *script;
	const char *table;
} scr_enc_cfg_t;

scr_enc_cfg_t *scr_enc_cfg_init(int argc, char **argv);

void scr_enc_cfg_uinit(scr_enc_cfg_t *cfg);

#endif /* SCRIPT_DECODING_CFG_H_ */
