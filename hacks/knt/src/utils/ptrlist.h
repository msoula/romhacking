/*
 * ptrlist.h
 *
 *  Created on: 26 nov. 2014
 *      Author: rid
 */

#ifndef PTRLIST_H_
#define PTRLIST_H_

#include <sys/types.h>

typedef struct _ptrlist
{
    void **data;    /*!< data array. */
    size_t size;    /*!< number of elements in array. */
    size_t count;   /*!< number of occupied elements. */
} ptrlist_t;

void ptrlist_init(ptrlist_t *list);
void ptrlist_init_size(ptrlist_t *list, size_t size);
void ptrlist_uninit(ptrlist_t *list);

int ptrlist_add(ptrlist_t *list, void *item);
int ptrlist_set(ptrlist_t *list, void *item, int idx);
int ptrlist_remove(ptrlist_t *list, int idx, void **item);
int ptrlist_remove_item(ptrlist_t *list, void *item);
int ptrlist_remove_all_items(ptrlist_t *list, void *item);

int ptrlist_contains(ptrlist_t *list, void *item);
int ptrlist_count(const ptrlist_t *list);
void *ptrlist_get(const ptrlist_t *list, int idx);

void ptrlist_sort(ptrlist_t *list, int (* itemcmp) (const void *, const void *));
int  ptrlist_to_file(ptrlist_t *list, const char *filename, const char *mode);

#endif /* PTRLIST_H_ */
