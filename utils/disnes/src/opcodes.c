/*
 * opcodes.c
 *
 *  Created on: 19 juin 2012
 *      Author: rid
 */

#include "opcodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

instruction_t instruction_set[0x100] = {
	{0x00, BRK, Implied},
	{0x01, ORA, Indirect_X},
	{0x02, UND, Undef},
	{0x03, UND, Undef},
	{0x04, UND, Undef},
	{0x05, ORA, Zero_Page},
	{0x06, ASL, Zero_Page},
	{0x07, UND, Undef},
	{0x08, PHP, Implied},
	{0x09, ORA, Immediate},
	{0x0A, ASL, Accumulator},
	{0x0B, UND, Undef},
	{0x0C, UND, Undef},
	{0x0D, ORA, Absolute},
	{0x0E, ASL, Absolute},
	{0x0F, UND, Undef},

	{0x10, BPL, Relative},
	{0x11, ORA, Indirect_Y},
	{0x12, UND, Undef},
	{0x13, UND, Undef},
	{0x14, UND, Undef},
	{0x15, ORA, Zero_Page_X},
	{0x16, ASL, Zero_Page_X},
	{0x17, UND, Undef},
	{0x18, CLC, Implied},
	{0x19, ORA, Absolute_Y},
	{0x1A, UND, Undef, },
	{0x1B, UND, Undef},
	{0x1C, UND, Undef},
	{0x1D, ORA, Absolute_X},
	{0x1E, ASL, Absolute_X},
	{0x1F, UND, Undef},

	{0x20, JSR, Absolute},
	{0x21, AND, Indirect_X},
	{0x22, UND, Undef},
	{0x23, UND, Undef},
	{0x24, BIT, Zero_Page},
	{0x25, AND, Zero_Page},
	{0x26, ROL, Zero_Page},
	{0x27, UND, Undef},
	{0x28, PLP, Implied},
	{0x29, AND, Immediate},
	{0x2A, ROL, Accumulator},
	{0x2B, UND, Undef},
	{0x2C, BIT, Absolute},
	{0x2D, AND, Absolute},
	{0x2E, ROL, Absolute},
	{0x2F, UND, Undef},

	{0x30, BMI, Relative},
	{0x31, AND, Indirect_Y},
	{0x32, UND, Undef},
	{0x33, UND, Undef},
	{0x34, UND, Undef},
	{0x35, AND, Zero_Page_X},
	{0x36, ROL, Zero_Page_X},
	{0x37, UND, Undef},
	{0x38, SEC, Implied},
	{0x39, AND, Absolute_Y},
	{0x3A, UND, Undef},
	{0x3B, UND, Undef},
	{0x3C, UND, Undef},
	{0x3D, AND, Absolute_X},
	{0x3E, ROL, Absolute_X},
	{0x3F, UND, Undef},

	{0x40, RTI, Implied},
	{0x41, EOR, Indirect_X},
	{0x42, UND, Undef},
	{0x43, UND, Undef},
	{0x44, UND, Undef},
	{0x45, EOR, Zero_Page},
	{0x46, LSR, Zero_Page},
	{0x47, UND, Undef},
	{0x48, PHA, Implied},
	{0x49, EOR, Immediate},
	{0x4A, LSR, Accumulator},
	{0x4B, UND, Undef},
	{0x4C, JMP, Absolute},
	{0x4D, EOR, Absolute},
	{0x4E, LSR, Absolute},
	{0x4F, UND, Undef},

	{0x50, BVC, Relative},
	{0x51, EOR, Indirect_Y},
	{0x52, UND, Undef},
	{0x53, UND, Undef},
	{0x54, UND, Undef},
	{0x55, EOR, Zero_Page_X},
	{0x56, LSR, Zero_Page_X},
	{0x57, UND, Undef},
	{0x58, CLI, Implied},
	{0x59, EOR, Absolute_Y},
	{0x5A, UND, Undef},
	{0x5B, UND, Undef},
	{0x5C, UND, Undef},
	{0x5D, EOR, Absolute_X},
	{0x5E, LSR, Absolute_X},
	{0x5F, UND, Undef},

	{0x60, RTS, Implied},
	{0x61, ADC, Indirect_X},
	{0x62, UND, Undef},
	{0x63, UND, Undef},
	{0x64, UND, Undef},
	{0x65, ADC, Zero_Page},
	{0x66, ROR, Zero_Page},
	{0x67, UND, Undef},
	{0x68, PLA, Implied},
	{0x69, ADC, Immediate},
	{0x6A, ROR, Accumulator},
	{0x6B, UND, Undef},
	{0x6C, JMP, Indirect},
	{0x6D, ADC, Absolute},
	{0x6E, ROR, Absolute},
	{0x6F, UND, Undef},

	{0x70, BVS, Relative},
	{0x71, ADC, Indirect_Y},
	{0x72, UND, Undef},
	{0x73, UND, Undef},
	{0x74, UND, Undef},
	{0x75, ADC, Zero_Page_X},
	{0x76, ROR, Zero_Page_X},
	{0x77, UND, Undef},
	{0x78, SEI, Implied},
	{0x79, ADC, Absolute_Y},
	{0x7A, UND, Undef},
	{0x7B, UND, Undef},
	{0x7C, UND, Undef},
	{0x7D, ADC, Absolute_X},
	{0x7E, ROR, Absolute_X},
	{0x7F, UND, Undef},

	{0x80, UND, Undef},
	{0x81, STA, Indirect_X},
	{0x82, UND, Undef},
	{0x83, UND, Undef},
	{0x84, STY, Zero_Page},
	{0x85, STA, Zero_Page},
	{0x86, STX, Zero_Page},
	{0x87, UND, Undef},
	{0x88, DEY, Implied},
	{0x89, UND, Undef},
	{0x8A, TXA, Implied},
	{0x8B, UND, Undef},
	{0x8C, STY, Absolute},
	{0x8D, STA, Absolute},
	{0x8E, STX, Absolute},
	{0x8F, UND, Undef},

	{0x90, BCC, Relative},
	{0x91, STA, Indirect_Y},
	{0x92, UND, Undef},
	{0x93, UND, Undef},
	{0x94, STY, Zero_Page_X},
	{0x95, STA, Zero_Page_X},
	{0x96, STX, Zero_Page_Y},
	{0x97, UND, Undef},
	{0x98, TYA, Implied},
	{0x99, STA, Absolute_Y},
	{0x9A, TXS, Implied},
	{0x9B, UND, Undef},
	{0x9C, UND, Undef},
	{0x9D, STA, Absolute_X},
	{0x9E, UND, Undef},
	{0x9F, UND, Undef},

	{0xA0, LDY, Immediate},
	{0xA1, LDA, Indirect_X},
	{0xA2, LDX, Immediate},
	{0xA3, UND, Undef},
	{0xA4, LDY, Zero_Page},
	{0xA5, LDA, Zero_Page},
	{0xA6, LDX, Zero_Page},
	{0xA7, UND, Undef},
	{0xA8, TAY, Implied},
	{0xA9, LDA, Immediate},
	{0xAA, TAX, Implied},
	{0xAB, UND, Undef},
	{0xAC, LDY, Absolute},
	{0xAD, LDA, Absolute},
	{0xAE, LDX, Absolute},
	{0xAF, UND, Undef},

	{0xB0, BCS, Relative},
	{0xB1, LDA, Indirect_Y},
	{0xB2, UND, Undef},
	{0xB3, UND, Undef},
	{0xB4, LDY, Zero_Page_X},
	{0xB5, LDA, Zero_Page_X},
	{0xB6, LDX, Zero_Page_Y},
	{0xB7, UND, Undef},
	{0xB8, CLV, Implied},
	{0xB9, LDA, Absolute_Y},
	{0xBA, TSX, Implied},
	{0xBB, UND, Undef},
	{0xBC, LDY, Absolute_X},
	{0xBD, LDA, Absolute_X},
	{0xBE, LDX, Absolute_Y},
	{0xBF, UND, Undef},

	{0xC0, CPY, Immediate},
	{0xC1, CMP, Indirect_X},
	{0xC2, UND, Undef},
	{0xC3, UND, Undef},
	{0xC4, CPY, Zero_Page},
	{0xC5, CMP, Zero_Page},
	{0xC6, DEC, Zero_Page},
	{0xC7, UND, Undef},
	{0xC8, INY, Implied},
	{0xC9, CMP, Immediate},
	{0xCA, DEX, Implied},
	{0xCB, UND, Undef},
	{0xCC, CPY, Absolute},
	{0xCD, CMP, Absolute},
	{0xCE, DEC, Absolute},
	{0xCF, UND, Undef},

	{0xD0, BNE, Relative},
	{0xD1, CMP, Indirect_Y},
	{0xD2, UND, Undef},
	{0xD3, UND, Undef},
	{0xD4, UND, Undef},
	{0xD5, CMP, Zero_Page_X},
	{0xD6, DEC, Zero_Page_X},
	{0xD7, UND, Undef},
	{0xD8, CLD, Implied},
	{0xD9, CMP, Absolute_Y},
	{0xDA, UND, Undef},
	{0xDB, UND, Undef},
	{0xDC, UND, Undef},
	{0xDD, CMP, Absolute_X},
	{0xDE, DEC, Absolute_X},
	{0xDF, UND, Undef},

	{0xE0, CPX, Immediate},
	{0xE1, SBC, Indirect_X},
	{0xE2, UND, Undef},
	{0xE3, UND, Undef},
	{0xE4, CPX, Zero_Page},
	{0xE5, SBC, Zero_Page},
	{0xE6, INC, Zero_Page},
	{0xE7, UND, Undef},
	{0xE8, INX, Implied},
	{0xE9, SBC, Immediate},
	{0xEA, NOP, Implied},
	{0xEB, UND, Undef},
	{0xEC, CPX, Absolute},
	{0xED, SBC, Absolute},
	{0xEE, INC, Absolute},
	{0xEF, UND, Undef},

	{0xF0, BEQ, Relative},
	{0xF1, SBC, Indirect_Y},
	{0xF2, UND, Undef},
	{0xF3, UND, Undef},
	{0xF4, UND, Undef},
	{0xF5, SBC, Zero_Page_X},
	{0xF6, INC, Zero_Page_X},
	{0xF7, UND, Undef},
	{0xF8, SED, Implied},
	{0xF9, SBC, Absolute_Y},
	{0xFA, UND, Undef},
	{0xFB, UND, Undef},
	{0xFC, UND, Undef},
	{0xFD, SBC, Absolute_X},
	{0xFE, INC, Absolute_X},
	{0xFF, UND, Undef}
};

/**
 * see http://www.obelisk.demon.co.uk/6502/addressing.html
 */
static const char *ADDRESSING_MODE_FORMATS[] = {
	"", "",
	"A",			 // Accumulator
	"#$%02X",		 // Immediate
	"($%02X,X)",	 // Indirect_X
	"($%02X,Y)",	 // Indirect_Y
	"$%02X",		 // Relative
	"$%02X",		 // Zero_Page
	"$%02X,X",		 // Zero_Page_X
	"$%02X,Y",		 // Zero_Page_Y
	"$%02X%02X",	 // Absolute
	"$%02X%02X,X",	 // Absolute_X
	"$%02X%02X,Y",	 // Absolute_Y
	"($%02X%02X)",	 // Indirect
};

static int ADDRESSING_MODE_BYTES[] = {
	0, 0, 0,
	1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2,
};

static const char *MNEMNONICS[] = {
	"ADC", "AND", "ASL",
	"BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRK", "BVC", "BVS",
	"CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY",
	"DEC", "DEX", "DEY",
	"EOR",
	"INC", "INX", "INY",
	"JMP", "JSR",
	"LDA", "LDX", "LDY", "LSR",
	"NOP",
	"ORA",
	"PHA", "PHP", "PLA", "PLP",
	"ROL", "ROR", "RTI", "RTS",
	"SBC", "SEC", "SED", "SEI", "STA", "STX", "STY",
	"TAX", "TAY", "TSX", "TXA", "TXS", "TYA",
	"UNDEFINED",
};

int instruction_param_number(instruction_t instruction) {
	return ADDRESSING_MODE_BYTES[instruction.add_mode];
}

const char *instruction_to_str(instruction_t instruction) {
	return MNEMNONICS[instruction.mnemonic];
}

const char *instruction_param_format(instruction_t instruction) {
	return ADDRESSING_MODE_FORMATS[instruction.add_mode];
}