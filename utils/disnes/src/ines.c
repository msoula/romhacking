/*
 * ines.c
 *
 *  Created on: 19 juin 2012
 *      Author: rid
 */

#include "ines.h"

#include <stdio.h>
#include <string.h>

mapper_t mappers[] = { { 0, "NROM, no mapper" }, { 1, "Nintendo MMC1" }, { 2, "UNROM switch" }, { 3, "CNROM switch" }, {
        4, "Nintendo MMC3" }, { 5, "Nintendo MMC5" }, { 6, "FFE F4xxx" }, { 7, "AOROM switch" }, { 8, "FFE F3xxx" }, {
        9, "Nintendo MMC2" }, { 10, "Nintendo MMC4" }, { 11, "ColorDreams chip" }, { 12, "FFE F6xxx" }, { 15,
        "100-in-1 switch" }, { 16, "Bandai chip" }, { 17, "FFE F8xxx" }, { 18, "Jaleco SS8806 chip" }, { 19,
        "Namcot 106 chip" }, { 20, "Nintendo DiskSystem" }, { 21, "Konami VRC4a" }, { 22, "Konami VRC2a" }, { 23,
        "Konami VRC2a" }, { 24, "Konami VRC6" }, { 25, "Konami VRC4b" }, { 32, "Irem G-101 chip" }, { 33,
        "Taito TC0190/TC0350" }, { 34, "32 KB ROM switch" }, { 64, "Tengen RAMBO-1 chip" }, { 65, "Irem H-3001 chip" },
        { 66, "GNROM switch" }, { 67, "SunSoft3 chip" }, { 68, "SunSoft4 chip" }, { 69, "SunSoft5 FME-7 chip" }, { 71,
                "Camerica chip" }, { 78, "Irem 74HC161/32-based" }, { 91, "Pirate HK-SF3 chip" },

        // dummy
        { 0xFF, "Unknown" } };

static mapper_t *mapper_by_number(u_int8_t number) {
    int i;
    mapper_t *mapper;
    for (i = 0, mapper = &mappers[i]; (number != mapper->number) && (0xFF != mapper->number); mapper = &mappers[i++])
        ;
    return mapper;
}

int ines_fprintf(FILE *f, ines_t *ines, fprintf_data_t *data) {
    fprintf(f, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");

    if (data && data->header) {
        data->header(f, data->header_data);
        fprintf(f, ";-------------------------------------------------------------------------------\n");
    }

    fprintf(f, "; PRG ROM              :    %dx16kB\n", ines->prg_rom);
    fprintf(f, "; CHR ROM              :    %dx8kB\n", ines->chr_rom);
    fprintf(f, "; Mirroring            :    %s\n", (ines->ctrl_byte_1 & 0x01) ? "Vertical" : "Horizontal");
    fprintf(f, "; SRAM                 :    %s\n", (ines->ctrl_byte_1 & 0x02) ? "Enabled" : "Disabled");
    fprintf(f, "; Trainer              :    %s\n", (ines->ctrl_byte_1 & 0x04) ? "Yes" : "No");
    fprintf(f, "; 4-screen VRAM layout :    %s\n", (ines->ctrl_byte_1 & 0x08) ? "Yes" : "No");

    u_int8_t mapper_number = (ines->ctrl_byte_2 & 0xF0) | ((ines->ctrl_byte_1 & 0xF0) >> 4);
    fprintf(f, "; Mapper               :    #%d (%s)\n", mapper_number, mapper_by_number(mapper_number)->name);

    if (data && data->footer) {
        fprintf(f, ";-------------------------------------------------------------------------------\n");
        data->footer(f, data->footer_data);
    }

    fprintf(f, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
    return 0;
}

int ines_load(ines_t *ines, FILE *f) {
    if (!f || !ines)
        return 1;

    unsigned char header[0x10] = { 0 };

    long pos = ftell(f);

    fseek(f, 0, SEEK_SET);
    fread(header, sizeof(unsigned char), 0x10, f);
    fseek(f, pos, SEEK_SET);

    memcpy(ines->header, header, 4);
    ines->prg_rom = header[4];
    ines->chr_rom = header[5];
    ines->ctrl_byte_1 = header[6];
    ines->ctrl_byte_2 = header[7];

    return 1;
}
