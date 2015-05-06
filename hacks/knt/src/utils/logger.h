/*
 * logger.h
 *
 *  Created on: 18 sept. 2011
 *      Author: rid
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>

#ifdef DEBUG
#define LOG_DEBUG(format, args...) fprintf(stdout, format" - %s:%d\n", ## args, __FILE__, __LINE__)
#else
#define LOG_DEBUG(format, args...)
#endif
#define LOG_INFO(format, args...) fprintf(stdout, format"\n", ## args)

#define LOG_WARN(format, args...) fprintf(stderr, format" - %s:%d\n", ## args, __FILE__, __LINE__)
#define LOG_ERROR(format, args...) fprintf(stderr, format" - %s:%d\n", ## args, __FILE__, __LINE__)

#endif /* LOGGER_H_ */
