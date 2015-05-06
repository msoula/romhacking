/*
 * playlet.h
 *
 *  Created on: 8 déc. 2014
 *      Author: rid
 */

#ifndef PLAYLET_H_
#define PLAYLET_H_

#include "array.h"
#include "cpu.h"
#include "file.h"
#include "ptrlist.h"

typedef struct _anim {
    u_int16_t length;
    array_t   data;
} anim_t;

typedef struct _playlet {
    u_int8_t  index;
    u_int8_t  bank;
    u_int16_t address;
    ptrlist_t anims;
} playlet_t;

typedef struct _shared_anim {
    u_int8_t  index;
    u_int16_t address;
    array_t   data;
    ptrlist_t anims;
} shared_anim_t;

playlet_t *playlet_init(u_int8_t index, u_int8_t bank, u_int16_t address);
void playlet_destroy(playlet_t *p);
int playlet_load_from_rom(playlet_t *p, FILE *rom, cpu_memory_t *cpu_mem);
int playlet_store_to_rom(playlet_t *p, FILE *rom, cpu_memory_t *cpu_mem);
int playlet_printf(playlet_t *p);

shared_anim_t *shared_anim_init(u_int8_t index, int16_t address);
void shared_anim_destroy(shared_anim_t *p);
int shared_anim_load_from_rom(shared_anim_t *p, FILE *rom, cpu_memory_t *cpu_mem);
int shared_anim_store_to_rom(shared_anim_t *p, FILE *rom, cpu_memory_t *cpu_mem);
int shared_anim_printf(shared_anim_t *p);

anim_t *anim_init();
void anim_destroy(anim_t *a);
int anim_load_from_rom(anim_t *a, FILE *rom);
int anim_store_to_rom(anim_t *a, FILE *rom);
int anim_printf(anim_t *a);

#endif /* PLAYLET_H_ */
