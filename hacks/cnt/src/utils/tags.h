/*
 * tags.h
 *
 *  Created on: 3 août 2011
 *      Author: rid
 */

#ifndef TAGS_H_
#define TAGS_H_

#include "utils/spc_utf8.h"

typedef enum {
	E_SCRIPT_TAG_DICO_START = 0,
	E_SCRIPT_TAG_DICO_STOP,
	E_SCRIPT_TAG_SPEED_UP,
	E_SCRIPT_TAG_JUMP_LINE,
	E_SCRIPT_TAG_NEW_FRAME,
	E_SCRIPT_TAG_WAITING_FOR,
	E_SCRIPT_TAG_SPEAK,
	E_SCRIPT_TAG_SHUT_UP,
	E_SCRIPT_TAG_END,
	E_SCRIPT_TAG_MAX
} E_SCRIPT_TAG_ID;

typedef struct {
	const char *pattern;
	unsigned int hex_value;
	int is_value;
	const char *format;
} tag_data_t;

extern tag_data_t script_tag_patterns[];

/**
 * @fn int is_script_tag(char *str, E_SCRIPT_TAG_ID *tag_id, int *length)
 * @brief Checks whether the given string starts with a SPC tag.
 *
 * @param u8str a pointer to the string to check.
 * @param tag_id the type of SPC tag.
 * @param length the tag length.
 * @return 1 if str starts with a SPC tag, 0 otherwise. *
 */
int is_code_tag(u_int32_t code, E_SCRIPT_TAG_ID *tag_id);

int is_script_tag(const char *str, E_SCRIPT_TAG_ID *tag_id, int *length);

/**
 * @fn int is_script_tag(char *str, E_SCRIPT_TAG_ID *tag_id, int *length)
 * @brief Checks whether the given string starts with a SPC tag.
 *
 * @param u8_str a pointer to the string to check.
 * @param tag_id the type of SPC tag.
 * @param length the tag length.
 * @return 1 if str starts with a SPC tag, 0 otherwise. *
 */
int is_u8_script_tag(const u_int32_t *u8_str, E_SCRIPT_TAG_ID *tag_id, int *length);

/**
 * @fn int script_tag_get_value(char *tag_str, int *tag_hex_value)
 * @brief Get the value attribute of the given tag.
 *
 * @param u8_str a string that starts with a SPC tag.
 * @param tag_hex_value the value attribute.
 * @return 0 if operation succeed, -1 otherwise.
 */
int script_tag_get_value(const u_int32_t *u8_str, int *tag_hex_value);

#endif /* TAGS_H_ */
