/*
 * mappers.c
 *
 *  Created on: 14 juil. 2012
 *      Author: rid
 */

#include "mappers.h"

#include "../utils/list.h"

static list_t *desc_list = NULL;

void mapper_register(mapper_desc_t *desc) {
	desc_list=list_append(desc_list, desc);
}

void mapper_unregister_all() {
	if (NULL == desc_list) list_free(desc_list);
}

mapper_desc_t *mapper_get_by_number(short number) {
	list_t *elem;
	for (elem = desc_list; elem; elem=list_next(elem)){
		mapper_desc_t *desc = (mapper_desc_t*)elem->data;
		if (number == desc->number){
			return desc;
		}
	}
	return NULL;
}
