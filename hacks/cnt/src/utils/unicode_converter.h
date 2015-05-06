/*
 * accent_jp.h
 *
 *  Created on: 9 sept. 2011
 *      Author: rid
 */

#ifndef ACCENT_JP_H_
#define ACCENT_JP_H_

#include <sys/types.h>

u_int32_t u8_to_codepoint(u_int32_t u8);
u_int32_t u16_to_codepoint(u_int32_t u16);
u_int32_t u32_to_codepoint(u_int32_t u32);

u_int32_t codepoint_to_u8(u_int32_t codepoint);
u_int32_t codepoint_to_u16(u_int32_t codepoint);
u_int32_t codepoint_to_u32(u_int32_t codepoint);

const char *codepoint_to_str(u_int32_t codepoint);

#endif /* ACCENT_JP_H_ */
