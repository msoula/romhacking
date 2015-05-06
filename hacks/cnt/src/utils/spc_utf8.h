/*
 * spc_utf8.h
 *
 *  Created on: 14 août 2011
 *      Author: rid
 */

#ifndef SPC_UTF8_H_
#define SPC_UTF8_H_

#include <sys/types.h>

/**
 * @fn u_int32_t next_u8_carac(const char *s, int *i)
 * @brief Returns next u8 characters of given string and increments given index.
 *
 * @param s	UTF-8 encoded string.
 * @param i	index on character to get.
 * @return a 32 bit integer representing an U8 character.
 */
u_int32_t next_u8_carac(const char *s, int *i);

/**
 * @fn u_int32_t *u8_convert_to_u8str(const char *s)
 * @brief Converts given standard string into u8str.
 *
 * @param s	standard string to convert.
 * @return an u8str or NULL if error.
 */
u_int32_t *u8_convert_to_u8str(const char *s);

/**
 * @fn char *u8_convert_to_str(const u_int32_t *s)
 * @brief Converts given u8str into standard string.
 *
 * @param s	u8str to convert.
 * @return a standard string, or NULL if error.
 */
const char *u8_convert_to_str(const u_int32_t *s);

/**
 * @fn size_t u8_strlen(const u_int32_t *s)
 * @brief Gets given u8str length (number of characters).
 *
 * @param s	u8str to treat.
 * @return number of characters.
 */
size_t u8_strlen(const u_int32_t *s);

/**
 * @fn int u8_memcmp(const u_int32_t *s1, const u_int32_t *s2, size_t length)
 * @brief Compares two u8str.
 *
 * @param s1	first u8str.
 * @param s2	second u8str
 * @param length	number of characters from starting offset to compare.
 * @return 0 if length first characters are identical, -1 otherwise.
 */
int u8_strcmp(const u_int32_t *s1, const u_int32_t *s2);
int u8_strncmp(const u_int32_t *s1, const u_int32_t *s2, size_t length);

/**
 * @fn int u8_starts_with(const u_int32_t *s1, const u_int32_t *s2)
 * @brief Checks if s1 starts with s2.
 *
 * @param s1	a u8str.
 * @param s2	a u8str.
 * @return 1 if s1 starts with s2, 0 otherwise.
 */
int u8_starts_with(const u_int32_t *s1, const u_int32_t *s2);

/**
 * @fn int u8_ends_with(const u_int32_t *s1, const u_int32_t *s2)
 * @brief Checks if s1 ends with s2.
 *
 * @param s1	a u8str.
 * @param s2	a u8str.
 * @return 1 if s1 ends with s2, 0 otherwise.
 */
int u8_ends_with(const u_int32_t *s1, const u_int32_t *s2);

u_int32_t *u8_strcpy(u_int32_t *dest, const u_int32_t *src);
u_int32_t *u8_strncpy(u_int32_t *dest, const u_int32_t *src, size_t n);

u_int32_t *u8_strcat(u_int32_t *dest, const u_int32_t *src);
u_int32_t *u8_strncat(u_int32_t *dest, const u_int32_t *src, size_t n);

#endif /* SPC_UTF8_H_ */
