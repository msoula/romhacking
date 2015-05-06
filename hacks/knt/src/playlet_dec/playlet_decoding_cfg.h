/*
 * script_encoding_cfg.h
 *
 *  Created on: 15 nov. 2011
 *      Author: rid
 */

#ifndef PLAYLET_DECODING_CFG_H_
#define PLAYLET_DECODING_CFG_H_

#define DEFAULT_CPU_MEM_VALUE   (NULL)
#define DEFAULT_ROM_VALUE       (NULL)
#define DEFAULT_XML_VALUE       (NULL)

typedef struct _playlet_dec_cfg {
	const char *cpu_mem;
	const char *rom;
    const char *xml;
} playlet_dec_cfg_t;

playlet_dec_cfg_t *playlet_dec_cfg_init(int argc, char **argv);

void playlet_dec_cfg_uinit(playlet_dec_cfg_t *cfg);

#endif /* PLAYLET_DECODING_CFG_H_ */
