/*
 * xml_playlet_reader.h
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#ifndef XML_PLAYLET_READER_H_
#define XML_PLAYLET_READER_H_

#include "utils/playlet.h"

int xml_playlet_reader_init(const char *filename);
void xml_playlet_reader_uinit();

int xml_playlet_reader_parse(ptrlist_t *shared_animations, ptrlist_t *playlets);

#endif /* XML_PLAYLET_READER_H_ */
