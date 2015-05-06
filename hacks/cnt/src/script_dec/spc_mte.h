/*
 * mte.h
 *
 *  Created on: 8 sept. 2011
 *      Author: rid
 */

#ifndef MTE_H_
#define MTE_H_

#include <stdio.h>
#include "../utils/cpu.h"

/**
 * @brief Get starting offset of given mte index.
 *
 * @param cpu		cpu memory representation.
 * @param index		mte index index.
 * @return mte index offset, -1 if error.
 */
long int get_mte_index_offset(cpu_memory_t *cpu, unsigned int index);

#endif /* MTE_H_ */
