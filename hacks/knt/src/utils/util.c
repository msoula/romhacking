/*
 * util.c
 *
 *  Created on: 3 sept. 2011
 *      Author: rid
 */

#include "util.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../utils/logger.h"

static char _buffer[BUFSIZ];

char *read_stdin(void) {
	char * result = fgets(_buffer, BUFSIZ, stdin);

	if (result != NULL) {
		char * lf = strchr(_buffer, '\n');
		if (lf != NULL)
			*lf = '\0';
	}

	return result;
}

void clear_stdin(void) {
    int c;

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int get_line_number(FILE *pFile) {
	int line_number = 0;
	char line[LINE_MAX_SIZE];

	if(!pFile) {
		LOG_ERROR("get_line_number() failed - file descriptor is NULL.\n");
		return -1;
	}
	fseek(pFile, 0, SEEK_SET);

	fgets(line, LINE_MAX_SIZE, pFile);
	while(!feof(pFile)) {
		line_number++;
		fgets(line, LINE_MAX_SIZE, pFile);
	}

	fseek(pFile, 0, SEEK_SET);
	return line_number;
}

char *trim_ending_jump_line(char *str) {
	char *end;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end >= str && (*end == '\n' || *end == '\r'))
		end--;

	*(end + 1) = '\0';

	return str;
}
