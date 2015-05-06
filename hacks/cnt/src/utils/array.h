/*
 * array.h
 *
 *  Created on: 9 déc. 2014
 *      Author: rid
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <sys/types.h>

typedef struct _array
{
    void *data;     /*!< data array. */
    size_t dsize;   /*!< data size. */
    size_t max;     /*!< maximum number of elements in array. */
    size_t count;   /*!< number of occupied elements. */
} array_t;

void array_init(array_t *array, size_t dsize);
void array_init_size(array_t *array, size_t dsize, size_t size);
void array_uninit(array_t *array);

int array_add(array_t *array, void *item);
int array_set(array_t *array, void *item, int idx);
int array_remove(array_t *array, int idx, void **item);

int array_contains(array_t *array, void *item);
int array_count(array_t *array);
void *array_get(array_t *array, int idx);

int array_to_file(array_t *array, const char *filename, const char *mode);

#endif /* ARRAY_H_ */
