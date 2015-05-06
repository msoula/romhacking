/*
 * array.c
 *
 *  Created on: 9 déc. 2014
 *      Author: rid
 */

#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCREMENT    32

static int array_grow(array_t *array);

void array_init(array_t *array, size_t dsize) {
    memset(array, 0, sizeof(*array));
    array->dsize = dsize;
}
void array_init_size(array_t *array, size_t dsize, size_t size) {
    memset(array, 0, sizeof(*array));
    array->dsize = dsize;
    if (size > 0) {
        array->max = size;
        array->data = malloc(array->dsize * array->max);
        memset(array->data, 0, array->dsize * array->max);
    }

}
void array_uninit(array_t *array) {
    if (array->data) {
        free(array->data), array->data = NULL;
    }
    array_init(array, array->dsize);
}

int array_add(array_t *array, void *item) {
    if(1 == array_grow(array))
        return 1;

    memcpy(array->data + (array->count * array->dsize), item, array->dsize);
    array->count++;
    return 0;
}
int array_remove(array_t *array, int idx, void **item) {
    int nmove;

    if(0 > idx || idx >= (int) array->count)
        return 1;

    if(NULL != item)
        *item = array_get(array, idx);

    nmove = (array->count - 1) - idx;
    if (nmove > 0) {
        memcpy(array->data + (idx * array->dsize), array->data + ((idx + 1) * array->dsize), nmove * array->dsize);
    }
    array->count--;
    return 0;

}

int array_contains(array_t *array, void *item) {
    int idx;
    int res = 0;

    if(NULL == array || NULL == array->data)
        return 0;

    for(idx = 0; idx < (int) array->count; idx++) {
        if(memcmp(array->data + (idx * array->dsize), item, array->dsize)) {
            res = 1;
            break;
        }
    }

    return res;
}
int array_count(array_t *array) {
    if(NULL == array || NULL == array->data)
        return 0;

    return array->count;
}
void *array_get(array_t *array, int idx) {
    if(NULL == array || NULL == array->data)
        return NULL;

    if(0 <= idx && (int) array->count > idx)
        return array->data + (idx * array->dsize);
    return NULL;
}

int array_to_file(array_t *array, const char *filename, const char *mode) {
    int res = 0;
    FILE *f;

    if(NULL == array || NULL == array->data)
        return 1;

    if(NULL == (f = fopen(filename, mode))) {
        return 1;
    }

    fwrite(array->data, array->dsize, array->count, f);

    fclose(f);

    return res;

}

/******************************************************************************/
int array_grow(array_t *array) {
    if(NULL == array->data) {
        if(0 == array->max)
            array->max = INCREMENT;
        array->count = 0;
        array->data = malloc(array->dsize * array->max);
        memset(array->data, 0, array->dsize * array->max);
    }
    else if(array->max == array->count){
        array->max *= 2;
        array->data = realloc(array->data, array->dsize * array->max);
        memset(array->data + (array->count * array->dsize), 0, (array->max - array->count) * array->dsize);
    }
    if(NULL == array->data)
        return 1;
    return 0;


}
