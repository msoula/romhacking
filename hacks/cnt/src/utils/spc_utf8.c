/*
 * spc_utf8.c
 *
 *  Created on: 14 août 2011
 *      Author: rid
 */

#include "spc_utf8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf8.h"
#include "utils/logger.h"

static size_t count_u8_caracs(const char *s) {

	int nb_bytes;
	size_t length = 0;
	const char *end = &(s[strlen(s)]);
	const char *current = s;
	while(current < end) {
		nb_bytes = u8_seqlen(current);
		if(nb_bytes < 0 || nb_bytes > 4) {
			LOG_ERROR("count_u8_caracs() failed - invalid character %c.", current[0]);
			exit(EXIT_FAILURE);
		}
		current += nb_bytes;
		length++;
	}
	return length;
}

//static size_t count_bytes(const u_int32_t *s) {
//	size_t length = 0;
//	int pos = 0;
//	while(s[pos] != '\0') {
//		if(s[pos] < 0x100) {
//			length++;
//		}
//		else if(s[pos] < 0x10000) {
//			length += 2;
//		}
//		else if(s[pos] < 0x1000000) {
//			length += 3;
//		}
//		else {
//			length += 4;
//		}
//		pos++;
//	}
//	return length;
//}

size_t u8_strlen(const u_int32_t *s) {
	size_t length = 0;
	while(s[length] != '\0')
		length++;
	return length;
}

u_int32_t next_u8_carac(const char *s, int *i) {

	u_int32_t u8_char;
	int nb_bytes = u8_seqlen(&s[*i]);
	switch(nb_bytes) {
	case 1:
		u8_char = (s[*i + 0] & 0xFF);
		break;
	case 2:
		u8_char = ((s[*i + 0] & 0xFF) * 0x0100 ) + (s[*i + 1] & 0xFF);
		break;
	case 3:
		u8_char = ((s[*i + 0] & 0xFF) * 0x100 * 0x100) + ((s[*i + 1] & 0xFF) * 0x100) + (s[*i + 2] & 0xFF);
		u8_char &= 0x00FFFFFF;
		break;
	case 4:
		u8_char = ((s[*i + 0] & 0xFF) * 0x100 * 0x100 * 0x100) + ((s[*i + 1] & 0xFF) * 0x100 * 0x100) + ((s[*i + 2] & 0xFF) * 0x100) + (s[*i + 3] & 0xFF);
		u8_char &= 0xFFFFFFFF;
		break;
	default:
		LOG_ERROR("next_u8_char() failed - invalid character %c.", s[*i + 0]);
		exit(EXIT_FAILURE);
		break;
	}
	(*i) += nb_bytes;
	return u8_char;
}

u_int32_t *u8_convert_to_u8str(const char *str) {

	int str_nb_u8_chars = count_u8_caracs(str);
	u_int32_t *p_u8_chars;
	if(NULL == (p_u8_chars = (u_int32_t *) calloc(str_nb_u8_chars + 1, sizeof(u_int32_t)))) {
		LOG_ERROR("ch_to_u8() failed - cannot allocate memory to initialize a new array of u8 chars.");
		return NULL;
	}

	int u8_pos = 0;
	const char *ch_pos = str;
	const char *ch_end = &str[strlen(str)];
	while(ch_pos != ch_end) {
		u_int32_t u8_char;
		int nb_bytes = u8_seqlen(ch_pos);
		switch(nb_bytes) {
		case 1:
			u8_char = (ch_pos[0] & 0xFF);
			break;
		case 2:
			u8_char = ((ch_pos[0] & 0xFF) * 0x0100 ) + (ch_pos[1] & 0xFF);
			break;
		case 3:
			u8_char = ((ch_pos[0] & 0xFF) * 0x100 * 0x100) + ((ch_pos[1] & 0xFF) * 0x100) + (ch_pos[2] & 0xFF);
			u8_char &= 0x00FFFFFF;
			break;
		case 4:
			u8_char = ((ch_pos[0] & 0xFF) * 0x100 * 0x100 * 0x100) + ((ch_pos[1] & 0xFF) * 0x100 * 0x100) + ((ch_pos[2] & 0xFF) * 0x100) + (ch_pos[3] & 0xFF);
			u8_char &= 0xFFFFFFFF;
			break;
		default:
			LOG_ERROR("ch_to_u8() failed - invalid character %c.", ch_pos[0]);
			exit(EXIT_FAILURE);
			break;
		}
		p_u8_chars[u8_pos++] = u8_char;
		ch_pos += nb_bytes;
	}
	p_u8_chars[u8_pos] = '\0';
	return p_u8_chars;
}

static char _u8_str[4 + 1] = {0};
const char *u8_convert_to_str(const u_int32_t *u8_str) {

	/** initializing u8_string */
	memset(_u8_str, '\0', 4 + 1);

	int str_idx = 0;
	size_t u8_idx;
	for(u8_idx = 0; u8_str[u8_idx] != '\0'; u8_idx++) {
		if(u8_str[u8_idx] < 0x100) {
			_u8_str[str_idx++] = (char) (u8_str[u8_idx] & 0xFF);
		}
		else if(u8_str[u8_idx] < 0x10000) {
			_u8_str[str_idx++] = (char) ((u8_str[u8_idx] & 0xFF00) >> 8);
			_u8_str[str_idx++] = (char) (u8_str[u8_idx] & 0xFF);
		}
		else if(u8_str[u8_idx] < 0x1000000) {
			_u8_str[str_idx++] = (char) ((u8_str[u8_idx] & 0xFF0000) >> 16);
			_u8_str[str_idx++] = (char) ((u8_str[u8_idx] & 0xFF00) >> 8);
			_u8_str[str_idx++] = (char) (u8_str[u8_idx] & 0xFF);
		}
		else {
			_u8_str[str_idx++] = (char) ((u8_str[u8_idx] & 0xFF000000) >> 24);
			_u8_str[str_idx++] = (char) ((u8_str[u8_idx] & 0xFF0000) >> 16);
			_u8_str[str_idx++] = (char) ((u8_str[u8_idx] & 0xFF00) >> 8);
			_u8_str[str_idx++] = (char) (u8_str[u8_idx] & 0xFF);
		}
	}
	_u8_str[str_idx] = '\0';
	return _u8_str;
}

int u8_starts_with(const u_int32_t *s1, const u_int32_t *s2) {
	return(u8_strncmp(s1, s2, u8_strlen(s2)) == 0);
}

int u8_ends_with(const u_int32_t *s1, const u_int32_t *s2) {
	size_t s1_len = u8_strlen(s1);
	size_t s2_len = u8_strlen(s2);
	if(s1_len < s2_len)
		return 0;

	return(u8_strncmp(&s1[s1_len - s2_len], s2, u8_strlen(s2)) == 0);
}

int u8_strcmp(const u_int32_t *s1, const u_int32_t *s2) {
	return u8_strncmp(s1, s2, u8_strlen(s2));
}

int u8_strncmp(const u_int32_t *s1, const u_int32_t *s2, size_t length) {
	return memcmp(s1, s2, length * sizeof(u_int32_t));
}

u_int32_t *u8_strcpy(u_int32_t *dest, const u_int32_t *src) {
	return u8_strncpy(dest, src, u8_strlen(dest));
}

u_int32_t *u8_strncpy(u_int32_t *dest, const u_int32_t *src, size_t n) {
	int future_length = u8_strlen(dest) + n;
	if(NULL == (dest = (u_int32_t *) memcpy(dest, src, n * sizeof(u_int32_t)))) {
		return NULL;
	}
	dest[future_length] = '\0';
	return dest;
}

u_int32_t *u8_strcat(u_int32_t *dest, const u_int32_t *src) {
	return u8_strncat(dest, src, u8_strlen(src));
}

u_int32_t *u8_strncat(u_int32_t *dest, const u_int32_t *src, size_t n) {
	int future_length = u8_strlen(dest) + n;
	if(NULL == (u_int32_t *) memcpy(&dest[u8_strlen(dest)], src, n * sizeof(u_int32_t))) {
		return NULL;
	}
	dest[future_length] = '\0';
	return dest;
}
