#include <stdio.h>
#include <stdlib.h>

#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif
#include <string.h>

#include "bank.h"
#include "disnes_cfg.h"
#include "ines.h"
#include "opcodes.h"
#include "utils/logger.h"

void rom_fprintf(FILE *f, void *userdata);

static int bank_no = 0;
int main(int argc, char **argv) {

	disnes_cfg_t *cfg = disnes_cfg_init(argc, argv);

	FILE *in;
	if(NULL == (in = fopen(cfg->rom, "rb"))) {
		LOG_ERROR("disnes - cannot open input rom file %s.", cfg->rom);
		return 1;
	}

	// creating output filename from input file
	int out_length;
	char out_filename[256] = {0};
	out_length = snprintf(out_filename, 256, "%s", cfg->rom);
	char *end = &out_filename[out_length];
	while(end && *(--end) != '.') ;
	if(end) {
		*end = '\0';
		out_length = strlen(out_filename);
	}

	u_int8_t page[cfg->bank_size];
	ines_t header;

	// loading header
	ines_load(&header, in);

	// covering banks
	for(bank_no = 0; bank_no < BANK_LIST_SIZE; bank_no++) {

		if(!cfg->banks[bank_no])
			continue;

		// skipping header
		fseek(in, 0x10 + (bank_no * cfg->bank_size), SEEK_SET);

		// reading one page
		fread(page, sizeof(u_int8_t), cfg->bank_size, in);

		// loading bank
		bank_t *bank = bank_init();
		bank_set_start_address(bank, cfg->bank_addr);
		bank_load(bank, page, cfg->bank_size);

		fprintf_data_t d = {
			rom_fprintf,
			basename(cfg->rom),
			NULL,
			NULL
		};

		snprintf(&out_filename[out_length], 256 - out_length, "_bank_%02X.6502", bank_no);

		FILE *out;
		LOG_DEBUG("disnes - opening output file %s...", out_filename);
		if(NULL == (out = fopen(out_filename, "wt"))) {
			LOG_ERROR("disnes - cannot open output file %s.", out_filename);
			break;
		}

		// printing bank disassembled
		ines_fprintf(out, &header, &d);
		bank_fprintf(out, bank);

		fclose(out);

		bank_destroy(bank);
	}

	fclose(in);

	disnes_cfg_uinit(cfg);

	return 0;
}

void rom_fprintf(FILE *f, void *userdata) {
	const char *rom_name = (const char *) userdata;
	fprintf(f, "; Rom filename : %s\n", rom_name);
	fprintf(f, "; Bank #%d\n", bank_no);
}
