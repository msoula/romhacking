/*
 * ines.c
 *
 *  Created on: 19 juin 2012
 *      Author: rid
 */

#include "ines.h"

#include "utils/logger.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INES_HEADER_SIZE	0x10
#define INES_HEADER_PRG_COUNT_IDX	0x04
#define INES_HEADER_CHR_COUNT_IDX	0x05
#define INES_HEADER_CTRL_BYTE_1_IDX	0x06
#define INES_HEADER_CTRL_BYTE_2_IDX	0x07

static u_int8_t ines_header[INES_HEADER_SIZE] = {0};

#define mapper_number(ines) \
	(((ines[INES_HEADER_CTRL_BYTE_1_IDX] & 0xF0) >> 4) | (ines[INES_HEADER_CTRL_BYTE_2_IDX] & 0xF0))

int ines_fprintf(FILE *f) {
	fprintf(f, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
	fprintf(f, "; PRG ROM              :	 %dx16kB\n", ines_header[INES_HEADER_PRG_COUNT_IDX]);
	fprintf(f, "; CHR ROM              :	 %dx8kB\n", ines_header[INES_HEADER_CHR_COUNT_IDX]);
	fprintf(f, "; Mapper number        :	 %02X\n", mapper_number(ines_header));
	fprintf(f, "; Mirroring            :	 %s\n", (ines_header[INES_HEADER_CTRL_BYTE_1_IDX] & 0x01) ? "Vertical" : "Horizontal");
	fprintf(f, "; SRAM                 :	 %s\n", (ines_header[INES_HEADER_CTRL_BYTE_1_IDX] & 0x02) ? "Enabled" : "Disabled");
	fprintf(f, "; Trainer              :	 %s\n", (ines_header[INES_HEADER_CTRL_BYTE_1_IDX] & 0x04) ? "Yes" : "No");
	fprintf(f, "; 4-screen VRAM layout :	 %s\n", (ines_header[INES_HEADER_CTRL_BYTE_1_IDX] & 0x08) ? "Yes" : "No");
	fprintf(f, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
	return 0;
}

u_int8_t ines_get_chr_count() {
	return ines_header[INES_HEADER_CHR_COUNT_IDX];
}

u_int8_t ines_get_mapper_number() {
	return mapper_number(ines_header);
}

u_int8_t ines_get_prg_count() {
	return ines_header[INES_HEADER_PRG_COUNT_IDX];
}

void ines_set_prg_count(u_int8_t x) {
	ines_header[INES_HEADER_PRG_COUNT_IDX] = x;
}

int ines_load(FILE *f) {
	if(!f) return 1;

	memset(ines_header, 0x00, INES_HEADER_SIZE);

	long pos = ftell(f);
	fseek(f, 0, SEEK_SET);
	fread(ines_header, sizeof(unsigned char), INES_HEADER_SIZE, f);
	fseek(f, pos, SEEK_SET);

	return (ines_header[0] == 'N' && ines_header[1] == 'E' && ines_header[2] == 'S' && ines_header[3] == 0x0A);
}

int ines_store(FILE *f) {
	int success = 1;
	if(!f) goto end;

	long pos = ftell(f);
	fseek(f, 0, SEEK_SET);

	if(1 != fwrite(ines_header, INES_HEADER_SIZE, 1, f)) {
		LOG_ERROR("ines_store - cannot ines header, errno=%d(%s).", errno, strerror(errno));
		goto end;
	}
	success = 0;

end:
	if(f) {
		fseek(f, pos, SEEK_SET);
	}
	return success;
}

void ines_unload() {
	memset(ines_header, 0x00, INES_HEADER_SIZE);
}
