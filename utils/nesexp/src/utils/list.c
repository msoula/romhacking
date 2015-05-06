/*
 * list.c
 *
 *  Created on: 14 juil. 2012
 *      Author: rid
 */

#include "list.h"
#include "logger.h"

list_t *list_new(void *data){
	list_t *new_elem=(list_t *) malloc (sizeof(list_t) * 1);
	new_elem->prev=new_elem->next=NULL;
	new_elem->data=data;
	return new_elem;
}

list_t * list_append(list_t *elem, void * data){
	list_t *new_elem=list_new(data);
	list_t *it=elem;
	if (elem==NULL) return new_elem;
	while (it->next!=NULL) it=list_next(it);
	it->next=new_elem;
	new_elem->prev=it;
	return elem;
}

list_t * list_prepend(list_t *elem, void *data){
	list_t *new_elem=list_new(data);
	if (elem!=NULL) {
		new_elem->next=elem;
		elem->prev=new_elem;
	}
	return new_elem;
}


list_t * list_concat(list_t *first, list_t *second){
	list_t *it=first;
	if (it==NULL) return second;
	while(it->next!=NULL) it=list_next(it);
	it->next=second;
	second->prev=it;
	return first;
}

list_t * list_free(list_t *list){
	list_t *elem = list;
	list_t *tmp;
	if (list==NULL) return NULL;
	while(elem->next!=NULL) {
		tmp = elem;
		elem = elem->next;
		free(tmp);
	}
	free(elem);
	return NULL;
}

list_t * list_remove(list_t *first, void *data){
	list_t *it;
	it=list_find(first,data);
	if (it) return list_remove_link(first,it);
	else {
		LOG_WARN("list_remove: no element with %p data was in the list", data);
		return first;
	}
}

int list_size(const list_t *first){
	int n=0;
	while(first!=NULL){
		++n;
		first=first->next;
	}
	return n;
}

void list_for_each(const list_t *list, void (*func)(void *)){
	for(;list!=NULL;list=list->next){
		func(list->data);
	}
}

void list_for_each2(const list_t *list, void (*func)(void *, void *), void *user_data){
	for(;list!=NULL;list=list->next){
		func(list->data,user_data);
	}
}

list_t *list_remove_link(list_t *list, list_t *elem){
	list_t *ret;
	if (elem==list){
		ret=elem->next;
		elem->prev=NULL;
		elem->next=NULL;
		if (ret!=NULL) ret->prev=NULL;
		free(elem);
		return ret;
	}
	elem->prev->next=elem->next;
	if (elem->next!=NULL) elem->next->prev=elem->prev;
	elem->next=NULL;
	elem->prev=NULL;
	free(elem);
	return list;
}

list_t *list_find(list_t *list, void *data){
	for(;list!=NULL;list=list->next){
		if (list->data==data) return list;
	}
	return NULL;
}

list_t *list_find_custom(list_t *list, int (*compare_func)(const void *, const void*), void *user_data){
	for(;list!=NULL;list=list->next){
		if (compare_func(list->data,user_data)==0) return list;
	}
	return NULL;
}

void * list_nth_data(const list_t *list, int index){
	int i;
	for(i=0;list!=NULL;list=list->next,++i){
		if (i==index) return list->data;
	}
	LOG_ERROR("list_nth_data: no such index in list.");
	return NULL;
}

int list_position(const list_t *list, list_t *elem){
	int i;
	for(i=0;list!=NULL;list=list->next,++i){
		if (elem==list) return i;
	}
	LOG_ERROR("list_position: no such element in list.");
	return -1;
}

int list_index(const list_t *list, void *data){
	int i;
	for(i=0;list!=NULL;list=list->next,++i){
		if (data==list->data) return i;
	}
	LOG_ERROR("list_index: no such element in list.");
	return -1;
}

list_t *list_insert_sorted(list_t *list, void *data, int (*compare_func)(const void *, const void*)){
	list_t *it,*previt=NULL;
	list_t *nelem;
	list_t *ret=list;
	if (list==NULL) return list_append(list,data);
	else{
		nelem=list_new(data);
		for(it=list;it!=NULL;it=it->next){
			previt=it;
			if (compare_func(data,it->data)<=0){
				nelem->prev=it->prev;
				nelem->next=it;
				if (it->prev!=NULL)
					it->prev->next=nelem;
				else{
					ret=nelem;
				}
				it->prev=nelem;
				return ret;
			}
		}
		previt->next=nelem;
		nelem->prev=previt;
	}
	return ret;
}
