/*
 * spc_encoding.c
 *
 *  Created on: 4 sept. 2011
 *      Author: msoula
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "encoder.h"
#include "script_encoding_cfg.h"
#include "xml_script_reader.h"
#include "utils/cpu.h"
#include "utils/logger.h"
#include "utils/mte.h"
#include "utils/script.h"
#include "utils/spc_utf8.h"
#include "utils/table.h"
#include "utils/tags.h"
#include "utils/util.h"

static const size_t PRGBANKSIZ = 8192;

static cpu_memory_t *_cpu_memory = NULL;
static table_t      *_table_fr   = NULL;
static dictionary_t *_dict       = NULL;
static FILE         *_rom        = NULL;
static script_t      _script;

/**
 * @brief Initialize SPC encoder.
 * @return 0 if operation succeed, -1 otherwise.
 */
static int init_encoder(scr_enc_cfg_t *cfg);

static int run(scr_enc_cfg_t *cfg);

static int clear_script_data();
static int insert_script_patch(u_int8_t idx);

/**
 * @brief Free encoder's tools.
 */
static inline void uinit_encoder();

static int encode_mte(u_int16_t from_cpu_address, array_t *dict_indexes, ptrlist_t *dict_entries);

static int encode_script(u_int16_t from_cpu_address, u_int16_t offset);

/**
 * Write encoded data to rom file at banks $0E and $0F.
 */
static int write_to_rom(array_t *dict_indexes, ptrlist_t *dict_entries);
static int write_to_bank(u_int8_t bank, array_t *dict_indexes, ptrlist_t *dict_entries, int *script_idx);

static char *get_new_filename(const char *from_filename);

int main(int argc, char **argv) {

    scr_enc_cfg_t *cfg;
    if (NULL == (cfg = scr_enc_cfg_init(argc, argv))) {
        LOG_ERROR("main() - cannot load script encoding config from arguments.");
        exit(EXIT_FAILURE);
    }

    if (-1 == run(cfg)) {
        LOG_ERROR("main() - cannot decode spc playlets.");
        exit(EXIT_FAILURE);
    }

    scr_enc_cfg_uinit(cfg);

    exit(EXIT_SUCCESS);}

int run(scr_enc_cfg_t *cfg) {

    int idx;

    LOG_DEBUG("script_encoding_run() - beginning script encoding...");

    //Rappel des arguments d'entrée
    LOG_DEBUG("CPU Memory File: %s\n", cfg->cpu_mem);
    LOG_DEBUG("Table File     : %s\n", cfg->table);
    LOG_DEBUG("Dictionary File: %s\n", cfg->dict);
    LOG_DEBUG("Script File:     %s\n", cfg->script);

    if (-1 == init_encoder(cfg)) {
        LOG_ERROR("script_encoding_run() failed - cannot initializing encoding data.");
        return 1;
    }

    /* encoding mte */
    array_t dict_indexes;
    ptrlist_t dict_entries;
    if (-1 == encode_mte(0xA000, &dict_indexes, &dict_entries)) {
        LOG_ERROR("script_encoding_run() failed - cannot encode dictionary.");
        return 1;
    }

    /* compute encoded dictionary size */
    size_t dict_size = array_count(&dict_indexes) * 2;
    for(idx = 0; idx < ptrlist_count(&dict_entries); idx++) {
        dict_size += array_count(ptrlist_get(&dict_entries, idx));
    }

    /* encoding script */
    if (-1 == encode_script(0xA000, dict_size)) {
        LOG_ERROR("script_encoding_run() failed - cannot encode script.");
        return 1;
    }

    /* writing buffer into files */
    write_to_rom(&dict_indexes, &dict_entries);

    /* free memory */
    while(0 < ptrlist_count(&dict_entries)) {
        array_t *cur;
        ptrlist_remove(&dict_entries, 0, (void **) &cur);
        array_uninit(cur);
        free(cur);
    }
    ptrlist_uninit(&dict_entries);
    array_uninit(&dict_indexes);

    uinit_encoder();

    return 0;
}

/***************************** Private functions *****************************/

static inline int init_encoder(scr_enc_cfg_t *cfg) {

    if (NULL == (_cpu_memory = cpu_init(cfg->cpu_mem))) {
        LOG_ERROR("init_encoder() failed - cannot initialize new cpu memory from file '%s'.", cfg->cpu_mem);
        return 1;
    }

    if (NULL == (_table_fr = table_load_from_xml(cfg->table))) {
        LOG_ERROR("init_encoder() failed - cannot initialize a new table.");
        free(_cpu_memory), _cpu_memory = NULL;
        return 1;
    }

    if (NULL == (_dict = dictionary_load_from_txt(cfg->dict))) {
        LOG_ERROR("init_encoder() failed - cannot initialize a new mte list.");
        free(_cpu_memory), _cpu_memory = NULL;
        free(_table_fr), _table_fr = NULL;
        return 1;
    };

    /* clone input rom */
    char *output_rom = get_new_filename(cfg->rom);
    file_clone(output_rom, cfg->rom);

    if (NULL == (_rom = fopen(output_rom, "r+"))) {
        LOG_ERROR("init_encoder() failed - cannot open rom file in WRITE mode.");
        free(_dict), _dict = NULL;
        free(_cpu_memory), _cpu_memory = NULL;
        free(_table_fr), _table_fr = NULL;
        return 1;
    }
    free(output_rom);

    xml_script_reader_init(cfg->script);
    xml_script_reader_parse(&_script);
    xml_script_reader_uinit();

    script_sort_by_address(&_script);

    return 0;
}
/******************************************************************************/
static inline void uinit_encoder() {
    fclose(_rom), _rom = NULL;
    dictionary_uinit(_dict), _dict = NULL;
    table_free(_table_fr), _table_fr = NULL;
    cpu_uinit(_cpu_memory), _cpu_memory = NULL;
}
/******************************************************************************/
int encode_mte(u_int16_t from_cpu_address, array_t *dict_indexes, ptrlist_t *dict_entries) {

    int i;
    unsigned int current_offset;
    size_t dict_size = dictionary_size(_dict);
    u_int8_t tmp;

    /* création d'un array_t de u_int16_t de taille dict_size */
    array_init_size(dict_indexes, sizeof(u_int16_t), dict_size);

    /* création d'un ptrlist_t de u_int8_t* de taille dict_size */
    ptrlist_init(dict_entries);

    /* dictionnaire = liste des index puis les entrées
     * --> pour calculer les index on se positionne au niveau des entrées...
     *  */
    current_offset = from_cpu_address + (dict_size * 2);

    /* parcours du dictionnaire */
    for (i = 0; i < (int) dict_size; i++) {

        /* calcul et sauvegarde de l'index du mot en cours */
        u_int16_t pointer = current_offset & 0xFFFF;
        array_add(dict_indexes, &pointer);

        /* préparation de l'encodage de l'entrée du dictionnaire en cours de traitement */
        array_t *code;
        if (NULL == (code = (array_t *) malloc(sizeof(array_t)))) {
            LOG_ERROR("encode_mte() failed - cannot allocate memory.");
            return 1;
        }
        array_init(code, sizeof(u_int8_t));

        /* encodage... */
        if (-1 == encode(code, mte_u8str(dictionary_get(_dict, i)), NULL, _table_fr)) {
            LOG_ERROR("script_encoding_run() failed - an error occured while encoding string '%s'.",
                    u8_convert_to_str(mte_u8str(dictionary_get(_dict, i))));
            return 1;
        }

        /* add separator */
        tmp = (u_int8_t) (script_tag_patterns[E_SCRIPT_TAG_DICO_STOP].hex_value & 0xFF);
        array_add(code, &tmp);

        /* sauvegarde */
        ptrlist_add(dict_entries, code);

        current_offset += array_count(code);
    }

    return 0;
}

int encode_script(u_int16_t from_cpu_address, u_int16_t offset) {

    int       i;
    size_t    script_size = script_count(&_script);
    u_int32_t cur_offset;

    /* current offset indicates current position of script data in rom */
    cur_offset = from_cpu_address + offset;

    for (i = 0; i < (int) script_size; i++) {

        u_int16_t pointer;
        text_t *entry;

        /* getting next script entry. */
        entry = script_get_entry(&_script, i);

        if (-1 == encode(&(entry->code), entry->u8srt, _dict, _table_fr)) {
            LOG_ERROR("script_encoding_run() failed - an error occurred while encoding string '%s'.",
                    u8_convert_to_str(entry->u8srt));
            return 1;
        }

        /* reset current offset if bank overflow */
        if(PRGBANKSIZ <= (cur_offset + array_count(&(entry->code)) - from_cpu_address)) {
            cur_offset = from_cpu_address + offset;
        }

        pointer = cur_offset & 0xFFFF;
        fprintf(stdout, "*** script #%02X: $0x%04X - %d bytes\n", i, pointer, array_count(&(entry->code)));
        entry->cpu_address = pointer;

        cur_offset += array_count(&(entry->code));
    }

    return 0;
}

/******************************************************************************/

int write_to_rom(array_t *dict_indexes, ptrlist_t *dict_entries) {
    int      cur_entry_idx;
    u_int8_t cur_bank;
    u_int8_t bank_switch_idx;

    slot_t   slot;
    long int offset;

    /* save current offset */
    offset = file_get_current_position(_rom);

    /* store current slot data at $A000 (ugly stuff incoming) */
    slot = _cpu_memory->slots[1];

    /* first fill banks with data */
    bank_switch_idx = 0x00;
    cur_entry_idx   = 0;
    cur_bank        = 0x0E;
    while((int) script_count(&_script) > cur_entry_idx) {
        write_to_bank(cur_bank, dict_indexes, dict_entries, &cur_entry_idx);
        if(0x00 == bank_switch_idx) {
            bank_switch_idx = cur_entry_idx;
        }
        cur_bank++;
    }

    /* go to script pointers position */
    if (-1 == file_go_to(_rom, SCRIPT_POINTER_SO)) {
        LOG_ERROR("playlet_store_to_rom() failed - cannot go to position 0x%08X.", SCRIPT_POINTER_SO);
        return 1;
    }

    /* IMPORTANT: respect pointer order */
    script_sort_by_pointer_index(&_script);

    /* then write script pointers */
    cur_entry_idx = 0;
    while((int) script_count(&_script) > cur_entry_idx) {
        text_t *entry = script_get_entry(&_script, cur_entry_idx);
        fwrite(&entry->cpu_address, sizeof(u_int16_t), 1, _rom);
        cur_entry_idx++;
    }

    clear_script_data();
    insert_script_patch(bank_switch_idx);

    /* restore bank in memory */
    cpu_modify_slot(_cpu_memory, 1, slot);

    /* go back to current offset */
    if (-1 == file_go_to(_rom, offset)) {
        LOG_ERROR("playlet_store_to_rom() failed - cannot go to position 0x%08lX.", offset);
    }

    return 0;
}

int write_to_bank(u_int8_t bank, array_t *dict_indexes, ptrlist_t *dict_entries, int *script_idx) {
    int idx;
    u_int16_t cur_bank_size;

    /* load playlet's bank in memory */
    cpu_modify_slot(_cpu_memory, 1, (slot_t) {bank, 0x10 + (bank * PRGBANKSIZ), 0x10 + ((bank + 1) * PRGBANKSIZ) - 1});

    /* go to bank */
    if (-1 == file_go_to(_rom, cpu_get_offset_from_address(_cpu_memory, 0xA000))) {
        LOG_ERROR("playlet_store_to_rom() failed - cannot go to position 0x%08lX.",
                cpu_get_offset_from_address(_cpu_memory, 0xA000));
        return 1;
    }
    cur_bank_size = 0;

    /* start writing dictionary data */
    file_write_bytes(_rom, dict_indexes->data, dict_indexes->dsize, dict_indexes->count);
    cur_bank_size += (dict_indexes->dsize * dict_indexes->count);
    for(idx = 0; idx < ptrlist_count(dict_entries); idx++) {
        array_t *code = ptrlist_get(dict_entries, idx);
        file_write_bytes(_rom, code->data, code->dsize, code->count);
        cur_bank_size += (code->dsize * code->count);
    }

    /* then write and many script entries as possible */
    while(*script_idx < (int) script_count(&_script)) {

        text_t *entry = script_get_entry(&_script, *script_idx);

        fprintf(stdout, "*** ** script #%02X: $0x%04X - %d bytes\n", *script_idx, entry->cpu_address, array_count(&(entry->code)));

        if(PRGBANKSIZ <= cur_bank_size + (entry->code.dsize * entry->code.count)) {
            /* bank is full */
            fprintf(stdout, "*** bank cannot take script entry #%02X\n", *script_idx);
            break;
        }

        file_write_bytes(_rom, entry->code.data, entry->code.dsize, entry->code.count);
        cur_bank_size += (entry->code.dsize * entry->code.count);
        (*script_idx)++;
    }

    return 0;
}

/******************************************************************************/

int clear_script_data() {

    size_t size = SCRIPT_DATA_EO - MTE_POINTER_SO;
    char   chunk[size];

    memset(chunk, '\0', size);

    /* overwrite script with chunk */
    fseek(_rom, MTE_POINTER_SO, SEEK_SET);
    file_write_bytes(_rom, chunk, size, 1);

    return 0;
}

/******************************************************************************/

int insert_script_patch(u_int8_t idx) {

    const size_t FUNC_SO = 0xEF8D; /* inclusive */
    const size_t FUNC_EO = 0xEFDA; /* exclusive */

    const char BANKSWITCH_PATCH [] = {

            /* the code below does bank swap to moved function's new location */
            0xA4, 0x2C,       /* LDY $2A   # get current bank number */
            0xA9, 0x06,       /* LDA #$0E  # */
            0x20, 0x9E, 0x8B, /* JSR $8B9E # swap 8k bank #E @ $A000 */
            0x98,             /* TYA       */
            0x48,             /* PHA       # push previous bank number */
            0x20, 0x00, 0xA0, /* JSR $A000 # execute subroutine at $A000 */
            0x68,             /* PLA       # pull previous bank number */
            0x20, 0x9E, 0x8B, /* JSR $8B9E # swap 8k bank #? @A000 */
            0x60,             /* RTS       */

            /* @$8F8E : the code below set bank number before calling text display function */
            0xA9, 0x0F,       /* LDA #$0F  # */
            0xAE, 0x22, 0x04, /* LDX $0422 # */
            0xE0, idx,        /* CPX idx   # 0F bank switch if $0422 > idx */
            0xB0, 0x02,       /* BCS $02   # */
            0xA9, 0x0E,       /* LDA #$0E  # */
            0x20, 0x94, 0x8C, /* JSR $8C94 # */
            0x60,             /* RTS */
    };

    size_t func_size = FUNC_EO - FUNC_SO;
    char   func_data[func_size];

    /* go to function position */
    fseek(_rom, FUNC_SO, SEEK_SET);

    /* read function from there */
    memset(func_data, '\0', func_size);
    file_read_bytes(_rom, func_data, func_size, 1);

    /* write function to its new location */
    fseek(_rom, MTE_POINTER_SO, SEEK_SET);
    file_write_bytes(_rom, func_data, func_size, 1);

    /* erase function at its old location */
    fseek(_rom, FUNC_SO, SEEK_SET);
    memset(func_data, '\0', func_size);
    file_write_bytes(_rom, func_data, func_size, 1);

    /* go to function old position */
    fseek(_rom, FUNC_SO, SEEK_SET);

    /* write patch data */
    file_write_bytes(_rom, BANKSWITCH_PATCH, sizeof(BANKSWITCH_PATCH), 1);

    /* finally bypass text function call at 0xE14B */
    /* go to call position */
    u_int16_t addr = 0x8F8E;
    fseek(_rom, 0xE14B, SEEK_SET);
    file_write_bytes(_rom, &addr, sizeof(u_int16_t), 1);


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
    strcat(name, ".fra");
    return name;
}
