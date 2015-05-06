/*
 * script_decoding.c
 *
 *  Created on: 4 sept. 2011
 *      Author: msoula
 */

#include <stdlib.h>
#include <string.h>

#include "script_decoding_cfg.h"
#include "spc_mte.h"
#include "xml_script_writer.h"
#include "spcdef.h"
#include "utils/cpu.h"
#include "utils/file.h"
#include "utils/logger.h"
#include "utils/pointers.h"
#include "utils/table.h"
#include "utils/tags.h"
#include "utils/unicode_converter.h"
#include "utils/util.h"

static table_t *_table_jp = NULL;
static cpu_memory_t *_cpu_memory = NULL;
static FILE *_rom = NULL;

/**
 * @brief Initializes the script decoder tool.
 *
 * @param	table_filename	original table filepath.
 * @param	cpu_filename		cpu memory map filepath.
 * @param	rom_filename		original rom file.
 * @return	0 if operation succeed, -1 otherwise.
 */
static int init(const char *table_filename, const char *cpu_filename, const char *rom_filename);

static int script_decoding_run(scr_dec_cfg_t *cfg);

/**
 * @brief Uninitializes the script decoder tool.
 *
 * @return	0 if operation succeed, -1 otherwise.
 */
static int uninit();

static pointer_table_t *init_spc_pointers();

int main(int argc, char **argv) {

	scr_dec_cfg_t *cfg;
	if(NULL == (cfg = scr_dec_cfg_init(argc, argv))) {
		LOG_ERROR("main() - cannot load script decoding config from arguments.");
		exit(EXIT_FAILURE);
	}

	if(-1 == script_decoding_run(cfg)) {
		LOG_ERROR("main() - cannot decode spc script and dico.");
		exit(EXIT_FAILURE);
	}

	scr_dec_cfg_uinit(cfg);

	exit(EXIT_SUCCESS);
}

int script_decoding_run(scr_dec_cfg_t *cfg) {
	LOG_DEBUG("script_decoding_run() - Beginning script decoding...");

	if(-1 == init(cfg->table, cfg->cpu_mem, cfg->rom)) {
		LOG_ERROR("script_decoding_run() failed - cannot initializing encoding data.");
		return -1;
	}

	xml_script_writer_init(cfg->xml);

	pointer_table_t *p_pointer_table;
	if(NULL == (p_pointer_table = init_spc_pointers())) {
		LOG_ERROR("script_decoding_run() - cannot init table of pointers from spc rom.");
		goto error;
	}

	int pointer_idx = 0;
	int scene_idx;
	for(pointer_idx = 0; pointer_idx < (int) p_pointer_table->pointer_number; pointer_idx++) {

		u_int16_t nes_pointer = p_pointer_table->pointers[pointer_idx];

		/* getting new pointer data. */
		scene_idx = 0;

		int pos = 0;
		char pointed_text[BUFSIZ] = {0};

		/* go to text position*/
		if(-1 == file_go_to(_rom, cpu_get_offset_from_address(_cpu_memory, nes_pointer))) {
			LOG_ERROR("script_decoding_run() failed - cannot go to text position 0x%08lX.", cpu_get_offset_from_address(_cpu_memory, nes_pointer));
		}

		fprintf(stderr, "%04X pointe sur %08lX\n", nes_pointer, file_get_current_position(_rom));

		u_int8_t text_byte;
		long int dico_return_pos = 0;
		while((script_tag_patterns[E_SCRIPT_TAG_END].hex_value != (text_byte = file_load_byte(_rom)) || !pointers_is_in_table(p_pointer_table, cpu_get_address_from_offset(_cpu_memory, file_get_current_position(_rom))))
				&& file_get_current_position(_rom) <= 0xDE01) {	// change this magic number

			if(script_tag_patterns[E_SCRIPT_TAG_JUMP_LINE].hex_value == text_byte) {
				pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_JUMP_LINE].format);
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_DICO_START].hex_value == text_byte) {
				/* getting mte pointer offset */
				text_byte = file_load_byte(_rom);
				long int mte_index_offset = get_mte_index_offset(_cpu_memory, text_byte);
				dico_return_pos = file_get_current_position(_rom);
				if(-1 == file_go_to(_rom, mte_index_offset)) {
					LOG_ERROR("cannot go to mte indexes position 0x%08lX.", mte_index_offset);
					continue;
				}

				/* reading mte pointer */
				u_int16_t mte_pointers = file_load_byte(_rom);
				mte_pointers += (file_load_byte(_rom) * 0x100);
				long int mte_entry_offset = cpu_get_offset_from_address(_cpu_memory, mte_pointers);
				if(-1 == file_go_to(_rom, mte_entry_offset)) {
					LOG_ERROR("cannot go to mte indexes position 0x%08lX.", mte_entry_offset);
					continue;
				}
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_DICO_STOP].hex_value == text_byte) {
				/* go back to script */
				if(-1 == file_go_to(_rom, dico_return_pos)) {
					LOG_ERROR("cannot go to previous script position 0x%08lX.", dico_return_pos);
					continue;
				}
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_NEW_FRAME].hex_value == text_byte) {
				pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_NEW_FRAME].format);
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_SHUT_UP].hex_value == text_byte) {
				pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_SHUT_UP].format);
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_SPEAK].hex_value == text_byte) {
				pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_SPEAK].format);
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_SPEED_UP].hex_value == text_byte) {
				text_byte = file_load_byte(_rom);
				sprintf(&pointed_text[strlen(pointed_text)], script_tag_patterns[E_SCRIPT_TAG_SPEED_UP].format, text_byte);
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_WAITING_FOR].hex_value == text_byte) {
				text_byte = file_load_byte(_rom);
				pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_WAITING_FOR].format, text_byte);
			}
			else if(0xB0 <= text_byte && 0xBF >= text_byte) {
				u_int8_t carac_to_handa = HANDAKUTEN_CARAC[text_byte - 0xB0];
				u_int32_t *table_entry;
				if(NULL == (table_entry = table_get_entry(_table_jp, carac_to_handa))) {
					LOG_ERROR("cannot find entry for index %02X.", carac_to_handa);
					continue;
				}
				u_int32_t c1 = codepoint_to_u8(u8_to_codepoint(*table_entry) + 2);
				pos += sprintf(&pointed_text[pos], "%s", codepoint_to_str(c1));
			}
			else if(0xC0 <= text_byte && 0xF4 >= text_byte) {
				u_int32_t *table_entry;
				if(NULL == (table_entry = table_get_entry(_table_jp, text_byte & 0x7F))) {
					LOG_ERROR("cannot find entry for index %02X.", text_byte & 0x7F);
					continue;
				}
				u_int32_t c1 = codepoint_to_u8(u8_to_codepoint(*table_entry) + 1);
				pos += sprintf(&pointed_text[pos], "%s", codepoint_to_str(c1));
			}
			else if(script_tag_patterns[E_SCRIPT_TAG_END].hex_value == text_byte) {
				pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_END].format);
			}
			else {
				u_int32_t *table_entry;
				if(NULL == (table_entry = table_get_entry(_table_jp, text_byte))) {
					LOG_ERROR("cannot find entry for index %02X.", text_byte);
					continue;
				}
				pos += sprintf(&pointed_text[pos], u8_convert_to_str(table_entry));
			}
		}
		pos += sprintf(&pointed_text[pos], script_tag_patterns[E_SCRIPT_TAG_END].format);
		xml_script_writer_add_pointer_element(p_pointer_table->pointers[pointer_idx], pointed_text);
	}

	pointers_uinit(p_pointer_table);

	/* flushing xml file */
	xml_script_writer_to_file();
	xml_script_writer_uinit();

	uninit();
	return 0;

error:
	xml_script_writer_uinit();

	uninit();
	return -1;
}

/******************************************************************************/
static int init(const char *table_filename, const char *cpu_filename, const char *rom_filename) {
	if(NULL == (_table_jp = table_load_from_xml(table_filename))) {
	    LOG_ERROR("init() failed - cannot initialize new table from file '%s'.", table_filename);
	    return -1;
	}

	if(NULL == (_cpu_memory = cpu_init(cpu_filename))) {
	    LOG_ERROR("init() failed - cannot initialize new cpu memory from file '%s'.", cpu_filename);
	    free(_table_jp);
	    return -1;
	}

	if(NULL == (_rom = fopen(rom_filename, "rb"))) {
	    LOG_ERROR("init() failed - cannot open rom file '%s'.", rom_filename);
	    free(_table_jp);
	    free(_cpu_memory);
	    return -1;
	}
	return 0;
}
/******************************************************************************/
static int uninit() {
    table_free(_table_jp);
    cpu_uinit(_cpu_memory);
    fclose(_rom);
    return 0;
}
/******************************************************************************/
static pointer_table_t *init_spc_pointers() {
	pointer_table_t *p_pointer_table;

	/* afficher les octets entre deux offset */
	u_int8_t *pointers;
	if(NULL == (pointers = file_load_bytes(_rom, SCRIPT_POINTER_SO, SCRIPT_POINTER_EO))) {
		LOG_ERROR("script_decoding_run() failed - cannot load list of script pointers from 0x%04X to 0x%04X.", SCRIPT_POINTER_SO, SCRIPT_POINTER_EO);
		goto error;
	}

	if(NULL == (p_pointer_table = pointers_init(0, 2, (SCRIPT_POINTER_EO - SCRIPT_POINTER_SO) / 2, pointers))) {
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
