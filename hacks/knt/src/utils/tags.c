/*
 * tags.c
 *
 *  Created on: 3 août 2011
 *      Author: rid
 */

#include "tags.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/logger.h"
#include "utils/spc_utf8.h"

tag_data_t script_tag_patterns[] = {
		{NULL, 0xF5, 0, NULL},
		{NULL, 0xF6, 0, NULL},
		{"^<speed[[:space:]]*value[[:space:]]*=[[:space:]]*\"0x\\([[:xdigit:]]\\{2\\}\\)\"[[:space:]]*/>", 0xF7, 1, "<speed value=\"0x%02X\" />"},
		{"^<jump[[:space:]]*/>\\\n\\\?", 0xF8, 0, "<jump />\n"},
		{"^<clear[[:space:]]*/>\\\n\\\?", 0xF9, 0, "<clear />\n"},
		{"^<wait[[:space:]]*value[[:space:]]*=[[:space:]]*\"0x\\([[:xdigit:]]\\{2\\}\\)\"[[:space:]]*/>", 0xFA, 1, "<wait value=\"0x%02X\" />"},
		{"^<speak[[:space:]]*/>", 0xFD, 0, "<speak />"},
		{"^<shutup[[:space:]]*/>", 0xFE, 0, "<shutup />"},
		{"^<end[[:space:]]*/>\\\n\\\?", 0xFF, 0, "<end />\n"}
};

int is_code_tag(u_int32_t code, E_SCRIPT_TAG_ID *tag_id) {
	int found = 0;
	int tag_idx;
	for(tag_idx = 0; !found && tag_idx < E_SCRIPT_TAG_MAX; tag_idx++) {
		if(script_tag_patterns[tag_idx].hex_value == code) {
			found = 1;
			*tag_id = tag_idx;
		}
	}
	return found;
}

int is_script_tag(const char *str, E_SCRIPT_TAG_ID *tag_id, int *length) {
	int tag_idx;
	int found = 0;
	for(tag_idx = 0; !found && tag_idx < E_SCRIPT_TAG_MAX; tag_idx++) {
		if(script_tag_patterns[tag_idx].pattern) {
			regex_t reg_expression;
			regmatch_t reg_matches[1];
			if( 0 != regcomp(&reg_expression, script_tag_patterns[tag_idx].pattern, 0)) {
				LOG_ERROR("is_script_tag() failed - regcomp() returned a non-zero value for pattern '%s'.", script_tag_patterns[tag_idx].pattern);
				break;
			}
			if(0 == regexec(&reg_expression, str, 1, reg_matches, 0)) {
				*tag_id= (E_SCRIPT_TAG_ID) tag_idx;
				*length = reg_matches[0].rm_eo;
				found = 1;
			}
			regfree(&reg_expression);
		}
	}
	return found;
}

int is_u8_script_tag(const u_int32_t *u8_str, E_SCRIPT_TAG_ID *tag_id, int *length) {
	const char *str = u8_convert_to_str(u8_str);

	int tag_idx;
	int found = 0;
	for(tag_idx = 0; !found && tag_idx < E_SCRIPT_TAG_MAX; tag_idx++) {
		if(script_tag_patterns[tag_idx].pattern) {
			regex_t reg_expression;
			regmatch_t reg_matches[1];
			if( 0 != regcomp(&reg_expression, script_tag_patterns[tag_idx].pattern, 0)) {
				LOG_ERROR("is_u8_script_tag() failed - regcomp() returned a non-zero value for pattern '%s'.", script_tag_patterns[tag_idx].pattern);
				break;
			}
			if(0 == regexec(&reg_expression, str, 1, reg_matches, 0)) {
				*tag_id= (E_SCRIPT_TAG_ID) tag_idx;
				*length = reg_matches[0].rm_eo;
				found = 1;
			}
			regfree(&reg_expression);
		}
	}
	return found;
}


int script_tag_get_value(const u_int32_t *u8_str, int *tag_hex_value) {
	char *str = strdup(u8_convert_to_str(u8_str));

	char *first_quote_pos;
	if(NULL == (first_quote_pos= strpbrk(str, "\""))) {
		free(str);
		LOG_ERROR("script_tag_get_value() failed - cannot split string!");
		return -1;
	}
	sscanf(&first_quote_pos[1], "0x%02X", tag_hex_value);
	free(str);
	return 0;
}
