/*
 * pointers.c
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#include "pointers.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/logger.h"

/**
 * @brief Initialize a table of pointers.
 *
 * @param type		pointer type (0: LE/1: BE)
 * @param size		table size.
 * @param length	pointer size (in byte).
 * @return a pointer to new table of pointers, or NULL if error.
 */
static pointer_table_t* init_pointer_table(const int type, const size_t size, const size_t length);

pointer_table_t *pointers_init(const int type, const int pointer_length, const int pointer_number, const u_int8_t *p_data) {
	assert(type == 0 || type == 1);
	assert(p_data);

	pointer_table_t *p_table;
	if(NULL == (p_table = init_pointer_table(type, pointer_number, pointer_length))) {
		LOG_ERROR("pointers_init() failed - cannot create new pointer table.");
		return NULL;
	}

	/* loading table */
	int pointer_idx;
	for(pointer_idx = 0; pointer_idx < pointer_number; pointer_idx++) {
		int data_idx = pointer_idx * pointer_length;
		long int pointer = 0;
		/* loading byte array */
		if(!type) { // Little endian
			int idx;
			for(idx = 0; idx < pointer_length; idx++) {
				pointer *= 0x100;
				pointer += p_data[data_idx + (pointer_length - idx - 1)];
			}
		}
		else {	// big endian
			int idx;
			for(idx = 0; idx < pointer_length; idx++) {
				pointer *= 0x100;
				pointer += p_data[data_idx + idx];
			}
		}
		p_table->pointers[pointer_idx] = pointer;
	}

	return p_table;
}

void pointers_print(pointer_table_t *p_table) {
	assert(p_table);

	LOG_DEBUG("Pointer table content:\n");
	LOG_DEBUG("size: %d\n", (int) p_table->pointer_number);
	LOG_DEBUG("type: %s\n", (p_table->type) ? "Big endian" : "Little endian");
	int pointer_idx;
	for(pointer_idx = 0; pointer_idx < (int) p_table->pointer_number; pointer_idx++) {
		LOG_DEBUG("$%08lX\n", p_table->pointers[pointer_idx]);
	}
}

int pointers_is_in_table(pointer_table_t *p_table, long int pointer) {
	int found = 0;
	int pointer_idx;
	for(pointer_idx = 0; !found && pointer_idx < (int) p_table->pointer_number; pointer_idx++) {
		found = (p_table->pointers[pointer_idx] == pointer);
	}
//	LOG_DEBUG("\npointers_is_in_table(%04lX) ? --> %s\n", pointer, (found) ? "yes" : "no");
	return (found);
}

void pointers_uinit(pointer_table_t *p_table) {
	assert(p_table);

	free(p_table->pointers);
	free(p_table);
}

/*************************** Private functions ********************************/
static pointer_table_t* init_pointer_table(const int type, const size_t size, const size_t length) {
	pointer_table_t *p_table;

	if(NULL == (p_table = (pointer_table_t *) malloc(sizeof(pointer_table_t)))) {
		LOG_ERROR("init_pointer_table() failed - cannot allocate memory to initialize pointer to pointer_table_t.");
		return NULL;
	}

	if(NULL == (p_table->pointers = (long int *) calloc(size * length, sizeof(long int)))) {
		LOG_ERROR("init_pointer_table() failed - cannot allocate memory to initialize array of long int.");
		free(p_table);
		return NULL;
	}

	p_table->type = type;
	p_table->pointer_length = length;
	p_table->pointer_number = size;
	return p_table;
}
/******************************************************************************/
