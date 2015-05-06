/*
 *script.h
 *
 *  Created on: 3 août 2011
 *      Author: rid
 */

#ifndef XML_SCRIPT_H_
#define XML_SCRIPT_H_

#include <stdio.h>
#include <sys/types.h>

#include "array.h"
#include "ptrlist.h"

typedef struct {
	u_int32_t    *u8srt;       /*!< text as an array of unicode chars. */
	array_t       code;        /*!< text as code. */
	u_int16_t     cpu_address;
	unsigned int  pointer_idx;
} text_t;

typedef struct {
	ptrlist_t texts;
} script_t;

/**
 * @brief Initializes a structure that will contain xml script content.
 *
 * @return a new structure, or NULL if error.
 */
script_t *script_init(void);

/**
 * @brief Free xml script structure.
 *
 * @param script	script to free.
 * @return 0 if initialization succeed, -1 otherwise.
 */
int script_uinit(script_t *script);

/**
 * @brief Print the script on standard output.
 *
 * @param output_fd output file descriptor.
 */
void script_fprintf(script_t *script, FILE *output_fd);

size_t script_count(script_t *script);
void script_sort_by_address(script_t *script);
void script_sort_by_pointer_index(script_t *script);
text_t *script_get_entry(script_t *script, int idx);

/**
 * @brief Initializes a structure that will contain xml text content.
 *
 * @return a new structure, or NULL if error.
 */
text_t *text_init();

/**
 * @brief Free xml text structure.
 *
 * @param t text to free.
 * @return 0 if operation succeed, -1 otherwise.
 */
void text_destroy(text_t *t);

int text_store_to_file(text_t *t, FILE *rom);

/**
 * @brief Print the text.
 *
 * @param t text to print.
 * @param output_fd output file descriptor.
 */
void text_fprintf(text_t *t, FILE *output_fd);

#endif /* XML_SCRIPT_H_ */
