/*
 * script_encoding_cfg.h
 *
 *  Created on: 15 nov. 2011
 *      Author: rid
 */

#ifndef PLAYLET_ENCODING_CFG_H_
#define PLAYLET_ENCODING_CFG_H_

#define DEFAULT_CPU_MEM_VALUE   (NULL)
#define DEFAULT_ROM_VALUE       (NULL)
#define DEFAULT_XML_VALUE       (NULL)

typedef struct _playlet_enc_cfg {
	const char *cpu_mem;
	const char *rom;
    const char *xml;
} playlet_enc_cfg_t;

playlet_enc_cfg_t *playlet_enc_cfg_init(int argc, char **argv);

void playlet_enc_cfg_uinit(playlet_enc_cfg_t *cfg);

#endif /* PLAYLET_ENCODING_CFG_H_ */
