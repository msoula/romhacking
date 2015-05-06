/*
 * mmc3.c
 *
 *  Created on: 14 juil. 2012
 *      Author: rid
 */

#include "mappers.h"

#include <errno.h>
#include <string.h>

#define MMC3_CHR_ROM_BANK_SIZE	 8 * 1024
#define MMC3_PRG_ROM_BANK_SIZE	 8 * 1024

static int mmc3_expand(FILE *in, u_int8_t prg_16kb_banks, u_int8_t chr_8kb_banks);

mapper_desc_t mmc3_desc = {
	.id=MMC3,
	.number = 0x04,
	.name="Nintendo MMC3",
	.expand = mmc3_expand
};

/******************************************************************************/
int mmc3_expand(FILE *in, u_int8_t prg_16kb_banks, u_int8_t chr_8kb_banks) {
	int success = 1;
	u_int8_t empty_prg_bank[prg_16kb_banks * PRG_ROM_BANK_SIZE];
	u_int8_t last_prg_8kb_banks[2 * MMC3_PRG_ROM_BANK_SIZE];
	u_int8_t chr_chunk[chr_8kb_banks * CHR_ROM_BANK_SIZE];

	long int pos = ftell(in);

	// inializing empty prg rom to append
	memset(empty_prg_bank, 0x00, sizeof(empty_prg_bank));

	// seek to last but one bank of prg-rom
	if(-1 == fseek(in, 0x10 + ((prg_16kb_banks -1) * PRG_ROM_BANK_SIZE), SEEK_SET)) {
		LOG_ERROR("mmc3_expand - cannot set the file position indicator to last but one prg rom banks, errno=%d(%s).", errno, strerror(errno));
		goto end;
	}

	// store 2 last banks of prg data
	if(1 != fread(last_prg_8kb_banks, sizeof(last_prg_8kb_banks), 1, in)) {
		LOG_ERROR("mmc3_expand - cannot read last prg rom banks, strerror=%d(%s).", errno, strerror(errno));
		goto end;
	}

	// store chr data
	if(1 != fread(chr_chunk, sizeof(chr_chunk), 1, in)) {
		LOG_ERROR("mmc3_expand - cannot read chunk of chr rom, strerror=%d(%s).", errno, strerror(errno));
		goto end;
	}

	// seek end of prg-rom banks
	if(-1 == fseek(in, 0x10 + ((prg_16kb_banks - 1) * PRG_ROM_BANK_SIZE), SEEK_SET)) {
		LOG_ERROR("mmc3_expand - cannot set the file position indicator to last bank of prg rom, errno=%d(%s).", errno, strerror(errno));
		goto end;
	}

	// expansing rom
	if(1 != fwrite(empty_prg_bank, sizeof(empty_prg_bank), 1, in)) {
		LOG_ERROR("mmc3_expand - cannot write chunk of empty prg rom, errno=%d(%s).", errno, strerror(errno));
		goto end;
	}
	if(1 != fwrite(last_prg_8kb_banks, sizeof(last_prg_8kb_banks), 1, in)) {
		LOG_ERROR("mmc3_expand - cannot write last banks of prg rom, errno=%d(%s).", errno, strerror(errno));
		goto end;
	}
	if(1 != fwrite(chr_chunk, sizeof(chr_chunk), 1, in)) {
		LOG_ERROR("mmc3_expand - cannot write chunk of chr rom, errno=%d(%s).", errno, strerror(errno));
		goto end;
	}

	success = 0;

end:
	if(-1 == fseek(in, pos, SEEK_SET)) {
		LOG_ERROR("mmc3_expand - cannot set the file position indicator to beginning of rom file, errno=%d(%s).", errno, strerror(errno));
	}

	return success;
}
