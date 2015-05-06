/*
 * util.h
 *
 *  Created on: 3 sept. 2011
 *      Author: rid
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>

#define LINE_MAX_SIZE	(256)
/**
 * @brief Read one line (terminated by '\n') from stdin.
 *
 * @return line read from stdin.
 */
char *read_stdin(void);

/**
 * @brief Clear standard input.
 */
void clear_stdin(void);

/**
 * \fn int get_line_number(FILE *pFile)
 * \brief Get the number of lines within given file.
 *
 * \param pFile file descriptor to check.
 * \return the number of lines within the pFile, or -1 on error;
 */
int get_line_number(FILE *pFile);

/**
 * @brief Trim trailing jump line characters.
 *
 * @param str	string to trim.
 * @return a pointer to trimmed string.
 */
char *trim_ending_jump_line(char *str);

#endif /* UTIL_H_ */
