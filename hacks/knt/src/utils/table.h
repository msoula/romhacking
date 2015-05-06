/*
 * table.h
 *
 *  Created on: 25 juil. 2011
 *      Author: rid
 */

#ifndef TABLE_H_
#define TABLE_H_

#include "utils/array.h"
#include <sys/types.h>

/**
 * \struct table_entry_t
 * \brief Structure containing table entry.
 *
 * A table_entry_t consists in one index and its value in table.
 */
struct _table_entry {
	unsigned int index;
	u_int32_t *value;
};
typedef struct _table_entry table_entry_t;

struct _table {
	table_entry_t *entries;
	int length;
};
typedef struct _table table_t;

/**
 * \fn table_t *table_load_from_tbl(const char *filepath)
 * \brief Load table data from given table file.
 *
 * @param filepath	path of file containing table to load.
 * @return 0 if operation succeed, -1 otherwise.
 */
table_t *table_load_from_tbl(const char *filepath);

/**
 * \fn table_t *table_load_from_xml(const char *filepath)
 * \brief Load table data from given xml table file.
 *
 * @param filepath	path of file containing table to load.
 * @return 0 if operation succeed, -1 otherwise.
 */
table_t *table_load_from_xml(const char *filepath);

/**
 * \fn inline void table_reset_content(void)
 * \brief Free this table's content.
 */
void table_free(table_t *table);


/**
 * \fn int table_get_hex_value(const char *str, unsigned int *hex_value, int *nb_cars)
 * \brief Fetch the best value in table to code the given string.
 *
 * \param table table to browse.
 * \param str   string to encode.
 * \param index hexadecimal value of the best match is placed in if value found.
 * \return the number of characters encoded.
 */
int table_get_best_value_index(const table_t * table, const u_int32_t *str, u_int8_t *index);

/**
 * \fn u_int32_t *table_get_entry(table_t * table, const unsigned int hex_value)
 * \brief Get table entry with given hexadecimal value.
 *
 * \param table		table to browse.
 * \param hex_value	hexadecimal value of entry to get.
 * \return a pointer to u8 string or NULL if not found.
 */
u_int32_t *table_get_entry(const table_t * table, const unsigned int hex_value);

/**
 * \fn void table_print(void)
 * \brief Display the content of table on standard output.
 */
void table_print(table_t *table);


#endif /* TABLE_H_ */
