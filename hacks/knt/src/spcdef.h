/*
 * spcdef.h
 *
 *  Created on: 11 sept. 2011
 *      Author: rid
 */

#ifndef SPCDEF_H_
#define SPCDEF_H_

#include <stdlib.h>
#include <sys/types.h>

/* playlet pointers starting offset */
#define PLAYLET_POINTER_SO   (0xFC19)
/* playlet pointers ending offset */
#define PLAYLET_POINTER_EO   (0xFC63)
/* global animation pointers starting offset */
#define SHARED_ANIM_POINTER_SO   (0xFA10)
/* shared animation pointers ending offset */
#define SHARED_ANIM_POINTER_EO   (0xFA32)

/* playlet's bank starting offset */
#define PLAYLET_BANKS_SO   (0xF08A)
/* playlet's bank ending offset */
#define PLAYLET_BANKS_EO   (0xF0AF)


/* script pointers starting offset */
#define SCRIPT_POINTER_SO   (0xF9C6)
/* script pointers ending offset */
#define SCRIPT_POINTER_EO   (0xFA10)

/* script data starting offset */
#define SCRIPT_DATA_SO   (0xC1A9)
/* script data ending offset */
#define SCRIPT_DATA_EO   (0xDE01)

/* original mte pointers starting offset */
#define MTE_POINTER_SO      (0xC010)
/* original mte pointers ending offset */
#define MTE_POINTER_EO      (0xC054)

/* original mte pointers starting offset */
#define MTE_DATA_SO      (0xC054)
/* original mte pointers ending offset */
#define MTE_DATA_EO      (0xC1A8)

/* handakuten index list starting offset */
#define HANDAKUTEN_CHAR_SO  (0xF008)
/* handakuten index list ending offset */
#define HANDAKUTEN_CHAR_EO  (HANDAKUTEN_CHAR_SO + 0x0A)

/* handakuten indexes */
extern u_int8_t HANDAKUTEN_CARAC[];

/* first part of translated mte */
#define MTE_PART_1_FILENAME   ("res/mte_fr_part1.txt")
/* second part of translated mte */
#define MTE_PART_2_FILENAME   ("res/mte_fr_part2.txt")

/* first part of translated script */
#define SCRIPT_PART_1_FILENAME   ("res/spc_fr_v2_part1.xml")
/* second part of translated script */
#define SCRIPT_PART_2_FILENAME   ("res/spc_fr_v2_part2.xml")

/* translated table */
#define TABLE_FR_FILENAME   ("res/table_fr.xml")
/* original table */
#define TABLE_JP_FILENAME   ("res/table_jp_02.xml")

/* cpu memory configuration */
#define CPU_MEM_FILENAME   ("res/cpu_memory.xml")

/* original SPC file checksum */
#define JP_SPC_MD5   ("e7502f63aac2f2b5f8c5dec1a501e650")

#define FREE_AND_NIL_IF_NULL(p) {\
   if(p) free(p), p = NULL; \
}

#endif /* SPCDEF_H_ */
