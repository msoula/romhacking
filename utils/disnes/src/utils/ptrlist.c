/*
 * ptrlist.c
 *
 *  Created on: 26 nov. 2014
 *      Author: rid
 */

#include "ptrlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INCREMENT    32
static int ptrlist_grow(ptrlist_t *list);

void ptrlist_init(ptrlist_t *list) {
    bzero(list, sizeof(*list));
}
void ptrlist_init_size(ptrlist_t *list, size_t size) {
    bzero(list, sizeof(*list));
    if (size > 0) {
        list->size = size;
        if (NULL == (list->data = malloc(sizeof(*list->data) * list->size))) {
            list->size = 0;
        }
    }
}
void ptrlist_uninit(ptrlist_t *list) {
    if (list->data) {
        free(list->data), list->data = NULL;
    }
    ptrlist_init(list);
}

int ptrlist_add(ptrlist_t *list, void *item) {
    if(1 == ptrlist_grow(list))
        return 1;

    list->data[list->count++] = item;
    return 0;
}
void *ptrlist_get(ptrlist_t *list, int idx) {
    if(NULL == list || NULL == list->data)
        return NULL;

    if(0 <= idx && (int) list->count > idx)
        return list->data[idx];
    return NULL;
}
int ptrlist_set(ptrlist_t *list, void *item, int idx) {
    if(0 > idx)
        return 1;

    if(idx >= (int) list->count)
        return ptrlist_add(list, item);

    if (1 == ptrlist_grow(list))
        return 1;

    /* open up a space for 1 element */
    bcopy(list->data + idx, list->data + idx + 1, (list->count - idx) * sizeof(*list->data));
    list->data[idx] = item;
    list->count++;
    return 0;
}
int ptrlist_remove(ptrlist_t *list, int idx, void **item) {
    int nmove;

    if(NULL == list->data || (int) list->count <= idx || 0 > idx)
        return 1;

    if(NULL != item)
        *item = list->data[idx];

    nmove = (list->count - 1) - idx;
    if (nmove > 0) {
        bcopy(list->data + (idx + 1), list->data + idx, nmove * sizeof(*list->data));
    }
    list->count--;

    return 0;
}
int ptrlist_remove_item(ptrlist_t *list, void *item) {
    int idx;

    if(NULL == list->data)
        return 1;

    idx = 0;
    while((int) list->count > idx) {
        if(item == list->data[idx]) {
            ptrlist_remove(list, idx, NULL);
        }
        idx++;
    }

    return 0;
}
int ptrlist_remove_all_items(ptrlist_t *list, void *item) {
    int idx;

    if(NULL == list->data)
        return 1;

    idx = 0;
    while((int) list->count > idx) {
        if(item == list->data[idx]) {
            ptrlist_remove(list, idx, NULL);
        }
        else {
            idx++;
        }
    }

    return 0;
}
int ptrlist_contains(ptrlist_t *list, void *item) {

    int idx;
    int res;

    if(NULL == list || NULL == list->data)
        return 0;

    res = 0;
    idx = 0;
    while((int) list->count > idx) {
        if(list->data[idx] == item) {
            res = 1;
            break;
        }
        idx++;
    }

    return res;
}

int ptrlist_count(ptrlist_t *list) {
    if(NULL == list || NULL == list->data)
        return 0;

    return list->count;
}

/******************************************************************************/
int ptrlist_grow(ptrlist_t *list) {
    if(NULL == list->data) {
        if(0 == list->size)
            list->size = INCREMENT;
        list->count = 0;
        list->data = malloc(sizeof(*list->data) * list->size);
    }
    else if(list->size == list->count){
        list->size *= 2;
        list->data = realloc(list->data, sizeof(*list->data) * list->size);
    }
    if(NULL == list->data)
        return 1;
    return 0;
}
