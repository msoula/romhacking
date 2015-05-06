/*
 * xml_playlet_writer.h
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#ifndef XML_PLAYLET_WRITER_H_
#define XML_PLAYLET_WRITER_H_

#include "utils/playlet.h"

int xml_playlet_writer_init(const char *filename);

int xml_playlet_writer_add_playlet(playlet_t *playlet);
int xml_playlet_writer_add_animation(anim_t *animation);
int xml_playlet_writer_add_shared_animation(shared_anim_t *shared_animation);

void xml_playlet_writer_to_file();

void xml_playlet_writer_uinit();


#endif /* XML_PLAYLET_WRITER_H_ */
