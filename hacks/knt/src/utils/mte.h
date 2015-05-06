/*
 * dict.h
 *
 *  Created on: 25 juil. 2011
 *      Author: rid
 */

#ifndef MTE_LIST_H_
#define MTE_LIST_H_

#define MTE_STARTING_OFFSET (0xC054)

#include "file.h"
#include "ptrlist.h"

typedef struct _mte{
    unsigned int  idx;
    u_int32_t    *entry;
} mte_t;

typedef struct _dictionary {
    ptrlist_t entries;    /*!< dictionary entries encoded in unicode. */
} dictionary_t;

/**
 * @brief Initialize a structure containing dictionary entries list.
 *
 * @return a pointer to a structure containing dictionary entries list, or NULL if error.
 */
dictionary_t *dictionary_init(void);

/**
 * @brief Free a given dictionary entries list structure.
 *
 * @param dict dictionary entries list to free.
 * @return 0 if operation succeed, -1 otherwise.
 */
int dictionary_uinit(dictionary_t * dict);

/**
 * @brief Loads dictionary entries list from text file.
 *
 * @param dict  a pointer to dictionary entries list structure to load.
 * @param filepath text file containing frequencies.
 * @return 0 if operation succeed, -1 otherwise.
 */
dictionary_t *dictionary_load_from_txt(const char *filepath);

/**
 * *** NOT IMPLEMENTED ***
 * @brief Loads dictionary entries list from nes file.
 *
 * @param dict a pointer to dictionary entries list structure to load.
 * @param filepath rom file.
 * @return 0 if operation succeed, -1 otherwise.
 * *** NOT IMPLEMENTED ***
 */
dictionary_t *dictionary_load_from_rom(const char *filepath);

/**
 * @brief Checks whether the given substring is frequencies list.
 *
 * @param str string to look for into the frequencies list.
 * @return the entries found, NULL otherwise..
 */
const mte_t *dictionary_find(const dictionary_t *dict, const u_int32_t *str);

/**
 * @brief Prints list of all frequencies.
 */
void dictionary_fprintf(const dictionary_t *dict, FILE *fd);

/**
 * @brief Get number of entries in dictionary.
 *
 * @param dict a dictionary.
 * @return the number of entries.
 */
size_t dictionary_size(const dictionary_t *dict);

/**
 * Get entry at given index.
 *
 * @param dict a dictionary.
 * @param idx the requested index.
 * @return an entry or NULL if not found.
 */
mte_t *dictionary_get(const dictionary_t *dict, int idx);

mte_t *mte_init(unsigned int idx, u_int32_t *u8str);
void mte_destroy(mte_t *mte);
void mte_fprintf(mte_t *mte, FILE *fd);

unsigned int mte_index(mte_t *mte);
size_t mte_length(const mte_t *mte);
const u_int32_t *mte_u8str(mte_t *mte);

#endif /* MTE_LIST_H_ */
