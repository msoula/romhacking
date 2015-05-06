/*
 * pointers.h
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#ifndef POINTERS_H_
#define POINTERS_H_

#include <sys/types.h>

struct _pointer_table {
	long int *pointers;		/*!< list of pointers. */
	size_t pointer_length;	/*!< pointer number of bytes. */
	size_t pointer_number;	/*!< number of pointers. */
	int type;				/*!< 0: Little Endian/1: Big Endian. */
};
typedef struct _pointer_table pointer_table_t;

/**
 * @brief Load pointers from array of bytes.
 *
 * @param type				pointer type.
 * @param pointer_length	pointer length (in bytes).
 * @param pointer_number	number of pointer to load.
 * @param data				array of bytes.
 * @return a pointer to pointer table structure, or NULL if error.
 */
pointer_table_t *pointers_init(const int type, const int pointer_length, const int pointer_number, const u_int8_t *data);

/**
 * @brief Display a given pointer table content.
 *
 * @param pointer_table	pointer table.
 */
void pointers_print(pointer_table_t *pointer_table);

/**
 * @brief Checks whether given pointer is in give pointer table.
 *
 * @param pointer_table	pointer table.
 * @param pointer		pointer to search.
 * @return 1 if pointer is found, 0 otherwise.
 */
int pointers_is_in_table(pointer_table_t *pointer_table, const long int pointer);

/**
 * @brief Free given pointer table.
 *
 * @param pointer_table	pointer table.
 */
void pointers_uinit(pointer_table_t *pointer_table);

#endif /* POINTERS_H_ */
