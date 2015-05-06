/*
 * cpu.h
 *
 *  Created on: 6 sept. 2011
 *      Author: rid
 */

#ifndef CPU_H_
#define CPU_H_

typedef struct {
	unsigned int bank_number;
	int starting_offset;
	int ending_offset;
} slot_t;

typedef struct {
	slot_t *slots;
	int nb_slots;
} cpu_memory_t;

/**
 * @brief Initialize CPU memory from file.
 *
 * @param filename	filename of file representing cpu memory.
 * @return a pointer to a structure representing cpu memory, or NULL if error.
 */
cpu_memory_t *cpu_init(const char *filename);

/**
 * @brief Modify CPU memory.
 *
 * @param cpu_memory pointer to cpu memory structure.
 * @param slot_idx slot index.
 * @param slot new bank data.
 */
int cpu_modify_slot(cpu_memory_t *cpu_memory, int slot_idx, slot_t slot);

/**
 * @brief Print content of given cpu memory.
 *
 * @param cpu_memory	pointer to cpu memory structure to display.
 */
void cpu_print(cpu_memory_t *cpu_memory);

/**
 * @brief Get offset corresponding to given cpu_address according given
 * cpu memory.
 *
 * @param cpu_memory	a pointer to a structure representing cpu memory.
 * @param cpu_address	cpu address to convert.
 * @return an offset, or -1 if error.
 */
long int cpu_get_offset_from_address(const cpu_memory_t *cpu_memory, const int cpu_address);

/**
 * @brief Get cpu address corresponding to given offset according given
 * cpu memory.
 *
 * @param cpu_memory	a pointer to a structure representing cpu memory.
 * @param offset		offset to convert.
 * @return a cpu address, or -1 if error.
 */
int cpu_get_address_from_offset(const cpu_memory_t *cpu_memory, const long int offset);

/**
 * @brief Free a structure representing cpu memory.
 *
 * @param cpu_memory	a pointer to structure to free.
 */
void cpu_uinit(cpu_memory_t *cpu_memory);

#endif /* CPU_H_ */
