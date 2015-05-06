/*
 * opcodes.h
 *
 *  Created on: 19 juin 2012
 *      Author: rid
 */

#ifndef OPCODES_H_
#define OPCODES_H_

#include <sys/types.h>

typedef enum _addressing_mode {
	// 0 byte addressing
	Implied = 0,
	Undef,
	Accumulator,

	// 1 byte addressing
	Immediate,
	Indirect_X,
	Indirect_Y,
	Relative,
	Zero_Page,
	Zero_Page_X,
	Zero_Page_Y,

	// 2 byte addressing
	Absolute,
	Absolute_X,
	Absolute_Y,
	Indirect,
} addressing_mode;

typedef enum _mnemonic {
	ADC,	// add memory to accumulator with carry
	AND,	// "AND" memory with accumulator
	ASL,	// shift left one bit (memory or accumulator)
	BCC,	// branch on carry clear
	BCS,	// branch on carry set
	BEQ,	// branch on result zero
	BIT,	// test bits in memorywith accumulator
	BMI,	// Branch on result minus
	BNE,	// branch if not equal
	BPL,	// branch onresult plus
	BRK,	// force interrupt
	BVC,	// branch on overflow clear
	BVS,	// branch on overflow set
	CLC,	// clear carry flag
	CLD,	// clear decimal mode
	CLI,	// clear interrupt disable bit
	CLV,	// clear overflow flag
	CMP,	// compare memory and accumulator
	CPX,	// compare memory and "X" register
	CPY,	// compare memory and "Y" register
	DEC,	// decrement memory
	DEX,	// decrement register "X"
	DEY,	// decrement register "Y"
	EOR,	// "EXCLUSIVE OR" memory with accumulator
	INC,	// increment memorty
	INX,	// increment register X
	INY,	// increment register Y
	JMP,	// jump to new location
	JSR,	// jump to new location saving return address
	LDA,	// load accumulator with memory
	LDX,	// load register X with memory
	LDY,	// load register Y with memory
	LSR,	// shift right one bit ( memory or accumulor)
	NOP,	// no operation
	ORA,	// "OR" memory with accumulor
	PHA,	// push accumulator on stack
	PHP,	// push processor status on stack
	PLA,	// pull accumulator from from stack
	PLP,	// pull processor status from stack
	ROL,	// rotate one bit left ( memory or accumulator)
	ROR,	// rotate one bit right ( memory or accumulator)
	RTI,	// return from interrupt
	RTS,	// return from subroutine
	SBC,	// subtract memory from accumulator with borrow
	SEC,	// set carry flag
	SED,	// set decimal mode
	SEI,	// set interrupt disable status
	STA,	// store accumulator in memory
	STX,	// store register "X" in memory
	STY,	// store register "Y" in memory
	TAX,	// transfer accumulator to register "X"
	TAY,	// transfer accumulator to register "Y"
	TSX,	// transfer stack pointer to register "X"
	TXA,	// transfer "X" to accumulator
	TXS,	// transfer register "X" to stack pointer
	TYA,	// transfer register "Y" to accumulator
	UND,	// Future expansion (unused)
} mnemonic;

typedef struct _instruction {
	u_int8_t opcode;
	mnemonic mnemonic;
	addressing_mode add_mode;
} instruction_t;

extern instruction_t instruction_set[];

const char *instruction_param_format(instruction_t instruction);
int instruction_param_number(instruction_t instruction);

const char *instruction_to_str(instruction_t instruction);

#endif /* OPCODES_H_ */
