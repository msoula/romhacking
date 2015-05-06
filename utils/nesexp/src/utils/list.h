/*
 * list.h
 *
 *  Created on: 14 juil. 2012
 *      Author: rid
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

struct _list {
	struct _list *next;
	struct _list *prev;
	void *data;
};

typedef struct _list list_t;

#define list_next(elem) ((elem)->next)

list_t *list_append(list_t *elem, void * data);
list_t *list_prepend(list_t *elem, void * data);
list_t *list_free(list_t *elem);
list_t *list_concat(list_t *first, list_t *second);
list_t *list_remove(list_t *first, void *data);
int list_size(const list_t *first);
void list_for_each(const list_t *list, void (*func)(void *));
void list_for_each2(const list_t *list, void (*func)(void *, void *), void *user_data);
list_t *list_remove_link(list_t *list, list_t *elem);
list_t *list_find(list_t *list, void *data);
list_t *list_find_custom(list_t *list, int (*compare_func)(const void *, const void*), void *user_data);
void *list_nth_data(const list_t *list, int index);
int list_position(const list_t *list, list_t *elem);
int list_index(const list_t *list, void *data);
list_t *list_insert_sorted(list_t *list, void *data, int (*compare_func)(const void *, const void*));


#endif /* LIST_H_ */
