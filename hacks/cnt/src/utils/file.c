/*
 * file.c
 *
 *  Created on: 8 sept. 2011
 *      Author: rid
 */


#include "file.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/logger.h"

int file_clone(const char *to_filename, const char *from_filename) {

    char *buf[BUFSIZ];
    FILE *in, *out;
    size_t len;

    /* open files */
    if(NULL == (in = fopen(from_filename, "r"))) {
        fprintf(stderr, "ERROR - cannot open '%s' in READ mode\n", from_filename);
        return 1;
    }
    if(NULL == (out = fopen(to_filename, "w"))) {
        fprintf(stderr, "ERROR - cannot open '%s' in WRITE mode\n", to_filename);
        return 1;
    }

    /* copy files */
    while(0 < (len = fread(buf, sizeof(char), BUFSIZ, in))) {
        if(len != fwrite(buf, sizeof(char), len, out)) {
            fprintf(stderr, "ERROR - cannot write %ld bytes into %s\n", len, to_filename);
        }
    }

    /* close files */
    fclose(out);
    fclose(in);

    return 0;
}

u_int8_t file_load_byte(FILE *p_file) {
	assert(p_file);

	u_int8_t byte;
	int nb_bytes_read;
	if(1 != (nb_bytes_read = fread(&byte, sizeof(u_int8_t), 1, p_file))) {
		if(-1 == nb_bytes_read) {
			LOG_ERROR("file_load() failed - fread() returned -1 - errno=%d (%s).", errno, strerror(errno));
			goto error;
		}
		else {
			LOG_ERROR("file_load() failed - can only read %d bytes on %d from file.", nb_bytes_read, 1);
			goto error;
		}
	}

	return byte;

error:
	return -1;
}

u_int8_t *file_load_bytes(FILE *p_file, const long int start, const long int end) {
	assert(p_file);

	u_int8_t *p_bytes;
	int length = end - start;
	if(NULL == (p_bytes = (u_int8_t *) malloc(sizeof(u_int8_t) * length))) {
		LOG_ERROR("file_load() failed - cannot allocate memory to initialize an array of %d u_int8_t.", length);
		return NULL;
	}

	long int current_pos;
	if(-1 == (current_pos = file_get_current_position(p_file))) {
		LOG_ERROR("file_load() failed - cannot get current position into file.");
		goto error;
	}

	if(-1 == file_go_to(p_file, start)) {
		LOG_ERROR("file_load() failed - cannot go to position %lX into file.", start);
		goto error;
	}

	int nb_bytes_read;
	if(length != (nb_bytes_read = fread(p_bytes, sizeof(u_int8_t), length, p_file))) {
		if(-1 == nb_bytes_read) {
			LOG_ERROR("file_load() failed - fread() returned -1 - errno=%d (%s).", errno, strerror(errno));
			goto error;
		}
		else {
			LOG_ERROR("file_load() failed - can only read %d bytes on %d from file.", nb_bytes_read, length);
			goto error;
		}
	}

	return p_bytes;

error:
	if(current_pos != -1) {
		if(-1 == file_go_to(p_file, current_pos)) {
			LOG_ERROR("file_load() failed - cannot go back to previous position %lX into file.", current_pos);
		}
	}
	free(p_bytes);
	return NULL;
}

long int file_get_current_position(FILE *p_file) {
	assert(p_file);

	long int current_pos;
	if(-1 == (current_pos = ftell(p_file))) {
		LOG_ERROR("file_get_current_position() failed - ftell() returned -1 - errno=%d (%s).", errno, strerror(errno));
		return -1;
	}
	return current_pos;
}

int file_go_to(FILE *p_file, const long int offset) {
	assert(p_file);

	if(-1 == fseek(p_file, offset, SEEK_SET)) {
		LOG_ERROR("file_go_to() failed - fseek() returned -1 - errno=%d (%s).", errno, strerror(errno));
		return -1;
	}
	return 0;
}

int file_read_bytes(FILE *p_file, const void *p_data, size_t size, size_t nmemb) {
    assert(p_file);

    int pos;
    int len;
    int remaining = size * nmemb;

    pos = 0;
    while(0 < remaining) {
        if(-1 == (len = fread(&(p_data[pos]), sizeof(char), remaining >= BUFSIZ ? BUFSIZ : remaining, p_file))) {
            fprintf(stderr, "ERROR - cannot read file\n");
            return 1;
        }
        remaining -= len;
        pos += len;
    }

    return 0;

}

int file_write_bytes(FILE *p_file, const void *p_data, size_t size, size_t nmemb) {
	assert(p_file);

    int pos;
    int len;
    int remaining = size * nmemb;

    pos = 0;
    while(0 < remaining) {
        if(-1 == (len = fwrite(&(p_data[pos]), sizeof(char), remaining >= BUFSIZ ? BUFSIZ : remaining, p_file))) {
            fprintf(stderr, "ERROR - cannot read PRG rom\n");
            return 1;
        }
        remaining -= len;
        pos += len;
    }

    return 0;
}

int file_write_bytes_at(FILE *p_file, const void *p_data, size_t size, size_t nmemb, const long int offset) {
	assert(p_file);

	size_t x;
	long int current_position = file_get_current_position(p_file);

	if(current_position != offset) {
		if(-1 == file_go_to(p_file, offset)) {
			LOG_ERROR("file_write_bytes_at() failed - impossible to go to offset %ld\n", offset);
			return -1;
		}
	}

	file_write_bytes(p_file, p_data, size, nmemb);

	if(current_position != offset) {
		if(-1 == file_go_to(p_file, current_position)) {
			LOG_ERROR("file_write_bytes_at() failed - impossible to go back to offset %ld\n", current_position);
			return -1;
		}
	}

	return 0;
}
