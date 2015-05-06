/*
 * main.c
 *
 *  Created on: 9 juil. 2012
 *      Author: msoula
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ines.h"
#include "mappers/alldescs.h"
#include "mappers/mappers.h"
#include "utils/logger.h"

static void init();
static void uninit();

int main(int argc, char **argv) {
	int exit_code = EXIT_FAILURE;
	const char *app_name = basename(argv[0]);
	FILE *in = NULL;
	mapper_desc_t *desc = NULL;

	init();

	if(argc != 2) {
		LOG_ERROR("Invalid arguments");
		goto end;
	}

	if(NULL == (in = fopen(argv[1], "rb+"))) {
		LOG_ERROR("%s - cannot open file '%s'.", app_name, argv[1]);
		goto end;
	}

	if(0 != ines_load(in)) {
		LOG_ERROR("%s - cannot load ines header from rom file '%s'.", app_name, argv[1]);
		goto end;
	}

	if(NULL != (desc = mapper_get_by_number(ines_get_mapper_number()))) {
		if(desc->expand) {
			// expanding rom
			LOG_INFO("%s - expanding rom with mapper %s...", app_name, desc->name);
			if(0 != desc->expand(in, ines_get_prg_count(), ines_get_chr_count())) {
				LOG_ERROR("%s - an error occured while expanding rom.", app_name);
				goto end;
			}

			// modifying iNes header
			ines_set_prg_count(ines_get_prg_count() * 2);
			if(0 != ines_store(in)) {
				LOG_ERROR("%s - cannot store iNes header in %s.", app_name, argv[1]);
				goto end;
			}
		}
		else {
			LOG_ERROR("%s - cannot call expand process for mapper %d(%s)", app_name, desc->number, desc->name);
			goto end;
		}
	}
	else {
		LOG_INFO("%s - cannot expand rom '%s' because it's mapper is not supported.", app_name, argv[1]);
	}

	exit_code = EXIT_SUCCESS;

end:
	ines_unload();

	if(in && EOF == fclose(in)) {
		LOG_ERROR("%s - cannot close file '%s'.", app_name, argv[1]);
	}
	uninit();
	exit(exit_code);
}

/******************************************************************************/
void init() {
	short i;

	LOG_INFO("Registering all mappers...");
	for (i=0;mapper_descs[i];i++){
		mapper_register(mapper_descs[i]);
	}
}
/******************************************************************************/
void uninit() {
	LOG_INFO("Unregistering all mappers...");
	mapper_unregister_all();
}
