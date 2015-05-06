/*
 * mte.c
 *
 *  Created on: 8 sept. 2011
 *      Author: rid
 */

#include "spc_mte.h"

#define MTE_INDEXES_STARTING_CPU_ADDRESS	(0xA000)

long int get_mte_index_offset(cpu_memory_t *p_cpu, unsigned int index) {
	return cpu_get_offset_from_address(p_cpu, MTE_INDEXES_STARTING_CPU_ADDRESS + (2 * index));
}
