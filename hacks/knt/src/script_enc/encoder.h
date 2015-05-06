/*
 * encoder.h
 *
 *  Created on: 3 août 2011
 *      Author: rid
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <sys/types.h>

#include "spcdef.h"
#include "utils/array.h"
#include "utils/mte.h"
#include "utils/table.h"

extern u_int8_t HANDAKUTEN_CARAC[];

/**
 * @brief Encode an UTF-8 string.
 *
 * @param dest encoded UTF-8 string.
 * @param src UTF-8 string to encode.
 * @param using_mte 1 to use mte, 0 to not use.
 * @return 0 if operation succeed, -1 otherwise.
 */
int encode(array_t *dest, const u_int32_t *src, dictionary_t *dict, table_t *table);

#endif /* ENCODER_H_ */
