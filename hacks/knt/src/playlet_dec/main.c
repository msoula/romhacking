/*
 * playlet_main.c
 *
 *  Created on: 4 sept. 2011
 *      Author: msoula
 */

#include <stdlib.h>
#include <string.h>

#include "playlet_decoding_cfg.h"
#include "xml_playlet_writer.h"
#include "../spcdef.h"
#include "../utils/cpu.h"
#include "../utils/file.h"
#include "../utils/logger.h"
#include "../utils/pointers.h"
#include "../utils/playlet.h"
#include "../utils/util.h"

static cpu_memory_t *_cpu_memory = NULL;
static FILE *_rom = NULL;

/**
 * @brief Initializes the script decoder tool.
 *
 * @param	cpu_filename		cpu memory map filepath.
 * @param	rom_filename		original rom file.
 * @return	0 if operation succeed, -1 otherwise.
 */
static int init(const char *cpu_filename, const char *rom_filename);

/**
 * @brief Uninitializes the script decoder tool.
 *
 * @return	0 if operation succeed, -1 otherwise.
 */
static int uninit();

static pointer_table_t *init_spc_pointers(u_int16_t from, u_int16_t to);
static int decode_playlets(u_int16_t from, u_int16_t to);
static int decode_shared_animations(u_int16_t from, u_int16_t to);

static int run(playlet_dec_cfg_t *cfg);
static u_int8_t playlet_get_bank_number(int playlet_index, u_int8_t *bank_nb);

int main(int argc, char **argv) {

    playlet_dec_cfg_t *cfg;
    if (NULL == (cfg = playlet_dec_cfg_init(argc, argv))) {
        LOG_ERROR("main() - cannot load script decoding config from arguments.");
        exit(EXIT_FAILURE);
    }

    if (-1 == run(cfg)) {
        LOG_ERROR("main() - cannot decode spc playlets.");
        exit(EXIT_FAILURE);
    }

    playlet_dec_cfg_uinit(cfg);

    exit(EXIT_SUCCESS);
}

int run(playlet_dec_cfg_t *cfg) {
    LOG_DEBUG("playlet_decoding_run() - Beginning playlet decoding...");

    if (-1 == init(cfg->cpu_mem, cfg->rom)) {
        LOG_ERROR("playlet_decoding_run() failed - cannot initialize decoding data.");
        return -1;
    }

    xml_playlet_writer_init(cfg->xml);

    if(0 != decode_shared_animations(SHARED_ANIM_POINTER_SO, SHARED_ANIM_POINTER_EO)) {
        goto error;
    }
    if(0 != decode_playlets(PLAYLET_POINTER_SO, PLAYLET_POINTER_EO)) {
        goto error;
    }

    /* flushing xml file */
    xml_playlet_writer_to_file();
    xml_playlet_writer_uinit();

    uninit();
    return 0;

error:
    xml_playlet_writer_uinit();

    uninit();
    return -1;
}

/******************************************************************************/
int init(const char *cpu_filename, const char *rom_filename) {
    if (NULL == (_cpu_memory = cpu_init(cpu_filename))) {
        LOG_ERROR("init() failed - cannot initialize new cpu memory from file '%s'.", cpu_filename);
        return -1;
    }

    if (NULL == (_rom = fopen(rom_filename, "rb"))) {
        LOG_ERROR("init() failed - cannot open rom file '%s'.", rom_filename);
        free(_cpu_memory);
        return -1;
    }
    return 0;
}
/******************************************************************************/
int uninit() {
    cpu_uinit(_cpu_memory);
    fclose(_rom);
    return 0;
}
/******************************************************************************/
pointer_table_t *init_spc_pointers(u_int16_t from, u_int16_t to) {
    pointer_table_t *p_pointer_table;

    /* afficher les octets entre deux offset */
    u_int8_t *pointers;
    if (NULL == (pointers = file_load_bytes(_rom, from, to))) {
        LOG_ERROR("init_playlet_pointers() failed - cannot load list of playlet pointers from 0x%04X to 0x%04X.", from, to);
        goto error;
    }

    if (NULL == (p_pointer_table = pointers_init(0, 2, (to - from) / 2, pointers))) {
        LOG_ERROR("cannot init pointer table from array of bytes.");
        goto error;
    }
    FREE_AND_NIL_IF_NULL(pointers)
    return p_pointer_table;

    error:
    FREE_AND_NIL_IF_NULL(pointers)
    return NULL;
}
/******************************************************************************/
u_int8_t playlet_get_bank_number(int playlet_index, u_int8_t *bank_nb) {

    long int offset = file_get_current_position(_rom);

    /* go to bank position*/
    if (-1 == file_go_to(_rom, PLAYLET_BANKS_SO + playlet_index)) {
        LOG_ERROR("playlet_get_bank_number() failed - cannot go to position 0x%08lX.", (long int) PLAYLET_BANKS_SO + playlet_index);
        return 1;
    }

    /* read bank number */
    *bank_nb = file_load_byte(_rom);

    /* restore position*/
    if (-1 == file_go_to(_rom, offset)) {
        LOG_ERROR("playlet_get_bank_number() failed - cannot go to position 0x%08lX.", offset);
    }

    return 0;

}
/******************************************************************************/

int decode_playlets(u_int16_t from, u_int16_t to) {

    /* load playlet pointers */
    pointer_table_t *p_pointer_table;
    if (NULL == (p_pointer_table = init_spc_pointers(from, to))) {
        LOG_ERROR("playlet_decoding_run() - cannot init table of pointers from spc rom.");
        return 1;
    }

    int pointer_idx = 0;
    for (pointer_idx = 0; pointer_idx < (int) p_pointer_table->pointer_number; pointer_idx++) {

        u_int8_t   bank_nb;
        playlet_t *playlet;
        u_int16_t  playlet_pointer = p_pointer_table->pointers[pointer_idx];

        /* read playlet bank number */
        playlet_get_bank_number(pointer_idx, &bank_nb);

        /* decode playlet */
        playlet = playlet_init(pointer_idx, bank_nb, playlet_pointer);
        if(0 == playlet_load_from_rom(playlet, _rom, _cpu_memory)) {
            xml_playlet_writer_add_playlet(playlet);
        }
        else {
            LOG_ERROR("ERROR - cannot load playlet data from rom");
        }
        playlet_destroy(playlet);

    }

    pointers_uinit(p_pointer_table);

    return 0;
}

int decode_shared_animations(u_int16_t from, u_int16_t to) {

    /* load playlet pointers */
    pointer_table_t *p_pointer_table;
    if (NULL == (p_pointer_table = init_spc_pointers(from, to))) {
        LOG_ERROR("decode_shared_animations() - cannot init table of shared anim pointers from spc rom.");
        return 1;
    }

    int pointer_idx = 0;
    for (pointer_idx = 0; pointer_idx < (int) p_pointer_table->pointer_number; pointer_idx++) {

        shared_anim_t *sa;
        u_int16_t      sa_pointer = p_pointer_table->pointers[pointer_idx];

        /* decode playlet */
        sa = shared_anim_init(pointer_idx, sa_pointer);
        if(0 == shared_anim_load_from_rom(sa, _rom, _cpu_memory)) {
            xml_playlet_writer_add_shared_animation(sa);
        }
        else {
            LOG_ERROR("ERROR - cannot load playlet data from rom");
        }
        shared_anim_destroy(sa);

    }

    pointers_uinit(p_pointer_table);

    return 0;
}
