/*
 * xml_script_writer.h
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#ifndef XML_SCRIPT_WRITER_H_
#define XML_SCRIPT_WRITER_H_

int xml_script_writer_init(const char *filename);

int xml_script_writer_add_pointer_element(int cpu_address, const char *text);

void xml_script_writer_to_file();

void xml_script_writer_uinit();


#endif /* XML_SCRIPT_WRITER_H_ */
