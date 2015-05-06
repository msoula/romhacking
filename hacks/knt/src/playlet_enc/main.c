/*
 * playlet_main.c
 *
 *  Created on: 4 sept. 2011
 *      Author: msoula
 */

#include <stdlib.h>
#include <string.h>

#include "playlet_encoding_cfg.h"
#include "xml_playlet_reader.h"
#include "../spcdef.h"
#include "../utils/cpu.h"
#include "../utils/file.h"
#include "../utils/logger.h"
#include "../utils/pointers.h"
#include "../utils/ptrlist.h"
#include "../utils/util.h"

static cpu_memory_t *_cpu_memory = NULL;
static FILE         *_rom = NULL;

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

static int   run(playlet_enc_cfg_t *cfg);
static char *get_new_filename(const char *from_filename);

int main(int argc, char **argv) {

    playlet_enc_cfg_t *cfg;
    if (NULL == (cfg = playlet_enc_cfg_init(argc, argv))) {
        LOG_ERROR("main() - cannot load playlet encoding config from arguments.");
        exit(EXIT_FAILURE);
    }

    if (-1 == run(cfg)) {
        LOG_ERROR("main() - cannot encode spc playlets.");
        exit(EXIT_FAILURE);
    }

    playlet_enc_cfg_uinit(cfg);

    exit(EXIT_SUCCESS);
}

int run(playlet_enc_cfg_t *cfg) {
    ptrlist_t playlets;
    ptrlist_t shared_animations;

    LOG_DEBUG("playlet_encoding_run() - Beginning playlet encoding...");

    if (0 != init(cfg->cpu_mem, cfg->rom)) {
        LOG_ERROR("playlet_encoding_run() failed - cannot initialize encoding data.");
        return -1;
    }

    xml_playlet_reader_init(cfg->xml);
    xml_playlet_reader_parse(&shared_animations, &playlets);
    xml_playlet_reader_uinit();

    /* treat and destroy shared animations */
    while(0 < ptrlist_count(&shared_animations)) {
        shared_anim_t *shared_animation;
        ptrlist_remove(&shared_animations, 0, (void **) &shared_animation);
        shared_anim_store_to_rom(shared_animation, _rom, _cpu_memory);
        shared_anim_destroy(shared_animation), shared_animation = NULL;
    }
    ptrlist_uninit(&shared_animations);

    /* treat and destroy shared animations */
    while(0 < ptrlist_count(&playlets)) {
        playlet_t *playlet;
        ptrlist_remove(&playlets, 0, (void **) &playlet);
        playlet_store_to_rom(playlet, _rom, _cpu_memory);
        playlet_destroy(playlet), playlet = NULL;
    }
    ptrlist_uninit(&playlets);

    uninit();
    return 0;

}

/******************************************************************************/

int init(const char *cpu_filename, const char *rom_filename) {
    if (NULL == (_cpu_memory = cpu_init(cpu_filename))) {
        LOG_ERROR("init() failed - cannot initialize new cpu memory from file '%s'.", cpu_filename);
        return -1;
    }

    /* clone input rom */
    char *output_rom = get_new_filename(rom_filename);
//    clone_file(output_rom, rom_filename);

    if (NULL == (_rom = fopen(rom_filename, "r+"))) {
        LOG_ERROR("init() failed - cannot open rom file '%s'.", output_rom);
        free(_cpu_memory);
        return 1;
    }
    free(output_rom);

    return 0;
}

/******************************************************************************/

int uninit() {
    cpu_uinit(_cpu_memory);
    fclose(_rom);
    return 0;
}

/******************************************************************************/

char *get_new_filename(const char *from_filename) {

    char *from_basename = basename(from_filename);
    char *name;
    if(NULL == (name = (char *) calloc(sizeof(char), strlen(from_basename) + 4 + 1))) { /* from_basename + ".new" + '\0' */
        fprintf(stderr, "ERROR - cannot create new filename from %s\n", from_basename);
        return NULL;
    }
    strcpy(name, from_basename);
    strcat(name, ".new");
    return name;
}
