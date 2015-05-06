/*
 * xml_script_reader.h
 *
 *  Created on: 10 déc. 2014
 *      Author: rid
 */

#ifndef XML_SCRIPT_READER_H_
#define XML_SCRIPT_READER_H_

#include "utils/script.h"

int xml_script_reader_init(const char *filename);
void xml_script_reader_uinit();

int xml_script_reader_parse(script_t *script);


#endif /* XML_SCRIPT_READER_H_ */
