/*
 * accent_jp.c
 *
 *  Created on: 9 sept. 2011
 *      Author: rid
 */

#include "unicode_converter.h"

#include <stdio.h>
#include <string.h>

u_int32_t u32_to_codepoint(u_int32_t u32) {
	return u32 & 0x001FFFFF;
}

u_int32_t u16_to_codepoint(u_int32_t u16) {
	u_int32_t high = (u16 & 0xFF00) / 0x100;
	u_int32_t low = (u16 & 0xFF);
	return (high - 0xD800) * 0x400 + (low - 0xDC00) + 0x10000;
}

u_int32_t u8_to_codepoint(u_int32_t u8) {
	if(u8 <= 0x7F) {
		return u8 & 0x7F;
	}
	else if(u8 <= 0xE000) {
		u_int32_t byte1 = (u8 & 0xFF00) / 0x100;
		u_int32_t byte2 = u8 & 0xFF;
		return (byte1 - 192) * 64 + byte2 - 128;
	}
	else if(u8 <= 0xF00000) {
		u_int32_t byte1 = (u8 & 0xFF0000) / 0x10000;
		u_int32_t byte2 = (u8 & 0x00FF00) / 0x100;
		u_int32_t byte3 = u8 & 0xFF;
		return (byte1 - 224) * 4096 + (byte2 - 128) * 64 + byte3 - 128;
	}
	else {
		u_int32_t byte1 = (u8 & 0xFF000000) / 0x1000000;
		u_int32_t byte2 = (u8 & 0x00FF0000) / 0x10000;
		u_int32_t byte3 = (u8 & 0x0000FF00) / 0x100;
		u_int32_t byte4 = u8 & 0xFF;
		return (byte1 - 240) * 262144 + (byte2 - 128) * 4096 + (byte3 - 128) * 64 + byte4 - 128;
	}
}

u_int32_t codepoint_to_u8(u_int32_t codepoint) {
	if(codepoint <= 0x7F) {
		return codepoint & 0x7F;
	}
	else if(codepoint <= 0x07FF) {
		u_int32_t byte1 = codepoint / 64 + 192;
		u_int32_t byte2 = codepoint % 64 + 128;
		return (byte1 << 8) + byte2;
	}
	else if((0x0800 <= codepoint && codepoint <= 0xD7FF) || (0xE000 <= codepoint && codepoint <= 0xFFFF)) {
		u_int32_t byte1 = codepoint / 4096 + 224;
		u_int32_t byte2 = (codepoint % 4096) / 64 + 128;
		u_int32_t byte3 = codepoint % 64 + 128;
		return (byte1 << 16) + (byte2 << 8) + byte3;
	}
	else {
		u_int32_t byte1 = codepoint / 262144 + 240;
		u_int32_t byte2 = (codepoint % 262144) / 4096 + 128;
		u_int32_t byte3 = (codepoint % 4096) / 64 + 128;
		u_int32_t byte4 = codepoint % 64 + 128;
		return (byte1 << 24) + (byte2 << 16) + (byte3 << 8) + byte4;
	}
}

u_int32_t codepoint_to_u16(u_int32_t codepoint) {
	u_int32_t high = (codepoint - 0x10000) / 0x400 + 0xD800;
	u_int32_t low = (codepoint - 0x10000) % 0x400 + 0xDC00;
	return (high << 16) + low;
}

u_int32_t codepoint_to_u32(u_int32_t codepoint) {
	return codepoint & 0x001FFFFF;
}

static char _codepoint_str[4] = {0};
const char *codepoint_to_str(u_int32_t codepoint) {
	memset(_codepoint_str, '\0', 4);
	_codepoint_str[0] = codepoint / 0x10000;
	_codepoint_str[1] = (codepoint / 0x100) & 0xFF;
	_codepoint_str[2] = codepoint & 0xFF;
	return _codepoint_str;
}
