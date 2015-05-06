/*
 * file.h
 *
 *  Created on: 8 sept. 2011
 *      Author: rid
 */

#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <sys/types.h>

int file_clone(const char *to_filename, const char *from_filename);

/**
 * @brief Loads an array of bytes from given file.
 *
 * @param file	file descriptor.
 * @param start	starting offset to load.
 * @param end	ending offset.
 * @return 	an array containing every bytes from starting offset to ending offset.
 */
u_int8_t *file_load_bytes(FILE *file, const long int start, const long int end);

/**
 * @brief Loads a bytes from given position.
 *
 * @param file	file descriptor.
 * @return	a byte.
 */
u_int8_t file_load_byte(FILE *file);

/**
 * @brief Gets current position into given file.
 *
 * @param file	file descriptor.
 * @return	current offset.
 */
long int file_get_current_position(FILE *file);

/**
 * @brief	Goes to given offset.
 * @param file		file descriptor.
 * @param offset	offset to go to.
 * @return	0 if operation succeed, -1 otherwise.
 */
int file_go_to(FILE *file, const long int offset);

/**
 * @brief   Reads a nmemb elements of data, each size bytes long, to the stream
 *          pointed to by file, obtaining them from the location given by data.
 * @param file  file descriptor.
 * @param data  array of data.
 * @param size  element size (in byte).
 * @param nmemb number of elements to read.
 * @return  0 if operation succeed, -1 otherwise.
 */
int file_read_bytes(FILE *file, const void *data, size_t size, size_t nmemb);

/**
 * @brief 	Writes a nmemb elements of data, each size bytes long, to the stream
 * 			pointed to by file, obtaining them from the location given by data.
 * @param file	file descriptor.
 * @param data	array of data.
 * @param size	element size (in byte).
 * @param nmemb	number of elements to write.
 * @return	0 if operation succeed, -1 otherwise.
 */
int file_write_bytes(FILE *file, const void *data, size_t size, size_t nmemb);

/**
 * @brief 	Writes a nmemb elements of data, each size bytes long, to the stream
 * 			pointed to by file at given offset, obtaining them from the location
 * 			given by data.
 * @param file		file descriptor.
 * @param data		array of data.
 * @param size		element size (in byte).
 * @param nmemb		number of elements to write.
 * @param offset	offset from where to write.
 * @return	0 if operation succeed, -1 otherwise.
 */
int file_write_bytes_at(FILE *file, const void *data, size_t size, size_t nmemb, const long int offset);

#endif /* FILE_H_ */
