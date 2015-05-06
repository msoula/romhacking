/*
 * mte.c
 *
 *  Created on: 25 juil. 2011
 *      Author: rid
 */

#include "mte.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "spc_utf8.h"

#define FREQ_LENGTH_MAX	(BUFSIZ)
#define LINE_SIZE_MAX	(256)

dictionary_t *dictionary_init(void) {
    dictionary_t *dict;

    LOG_DEBUG("Initializing dictionary...");
    if (NULL == (dict = (dictionary_t *) malloc(sizeof(dictionary_t)))) {
        LOG_ERROR("cannot allocate memory to initialize dictionary.");
        return NULL;
    }

    ptrlist_init(&(dict->entries));
    return dict;
}

int dictionary_uinit(dictionary_t *dict) {
    LOG_DEBUG("Freeing dictionary...");

    /* remove every animation data */
    while (0 < ptrlist_count(&(dict->entries))) {

        u_int32_t *cur = NULL;
        ptrlist_remove(&(dict->entries), 0, (void **) &cur);
        if (NULL != cur)
            free(cur);
    }

    ptrlist_uninit(&(dict->entries));
    free(dict);
    return 0;
}

dictionary_t *dictionary_load_from_txt(const char *filepath) {
    FILE *pFile;

    dictionary_t *dict;
    if (NULL == (dict = dictionary_init())) {
        LOG_ERROR("dictionary_load_from_txt() failed - cannot create dictionary\n");
        return NULL;
    }

    /* open file in READ only mode */
    if (NULL == (pFile = fopen(filepath, "r"))) {
        LOG_ERROR("dictionary_load_from_txt() failed - cannot open dictionary file '%s'\n", filepath);
        return NULL;
    }

    /* read content line by line */
    int idx = 0;
    char line[LINE_SIZE_MAX] = { 0 };
    while (NULL != fgets(line, LINE_SIZE_MAX, pFile)) {
        /* remove endline */
        line[strlen(line) - 1] = '\0';

        ptrlist_add(&(dict->entries), mte_init(idx++, u8_convert_to_u8str(line)));
    }

    if (0 != fclose(pFile)) {
        LOG_ERROR("dictionary_load_from_txt() failed() - cannot close dictionary file '%s' - errno=%d(%s)\n", filepath,
                errno, strerror(errno));
    }
    return dict;
}

dictionary_t *dictionary_load_from_rom(const char *filepath) {
    (void) filepath;

    return NULL;
}

const mte_t *dictionary_find(const dictionary_t *dict, const u_int32_t *str) {
    int idx;
    const mte_t *entry = NULL;

    size_t len = u8_strlen(str);

    /* find mte */
    for (idx = 0; NULL == entry && idx < ptrlist_count(&(dict->entries)); idx++) {
        mte_t *cur = (mte_t *) ptrlist_get(&(dict->entries), idx);
        size_t cur_len = u8_strlen(cur->entry);

        if (len >= cur_len) {
            if (!u8_strncmp(str, cur->entry, cur_len)) {
                entry = cur;
            }
        }
    }

    return entry;
}

void dictionary_fprintf(const dictionary_t *dict, FILE *fd) {
    int idx;

    for (idx = 0; idx < ptrlist_count(&(dict->entries)); idx++) {
        mte_fprintf((mte_t *) ptrlist_get(&(dict->entries), idx), fd);
    }
}

size_t dictionary_size(const dictionary_t *dict) {
    return ptrlist_count(&(dict->entries));
}

mte_t *dictionary_get(const dictionary_t *dict, int idx) {
    return (mte_t *) ptrlist_get(&(dict->entries), idx);
}

/******************************************************************************/

mte_t *mte_init(unsigned int idx, u_int32_t *u8str) {

    mte_t *mte;
    if (NULL == (mte = (mte_t *) malloc(sizeof(mte_t)))) {
        LOG_ERROR("cannot allocate memory to initialize a new mte.");
        return NULL;
    }
    mte->idx   = idx;
    mte->entry = u8str;
    return mte;

}
void mte_destroy(mte_t *mte) {
    if (mte->entry)
        free(mte->entry);
    free(mte);

}
void mte_fprintf(mte_t *mte, FILE *fd) {
    u_int32_t *cur = mte->entry;
    size_t len = u8_strlen(cur);

    fprintf(fd, "%ld: '", len);
    fwrite(cur, sizeof(u_int32_t), len, fd);
    fprintf(fd, "'\n");
}

unsigned int mte_index(mte_t *mte) {
    return mte->idx;
}
const u_int32_t *mte_u8str(mte_t *mte) {
    return mte->entry;
}
size_t mte_length(const mte_t *mte) {
    return u8_strlen(mte->entry);
}
