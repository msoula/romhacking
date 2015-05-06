/*
 * cpu.c
 *
 *  Created on: 6 sept. 2011
 *      Author: rid
 */

#include "cpu.h"

#include <libxml/xmlreader.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/logger.h"

#define XML_SLOT_ELEMENT_NAME		("slot")
#define XML_ATTR_BANK_NUMBER		("bank_number")
#define XML_ATTR_STARTING_OFFSET	("starting_offset")
#define XML_ATTR_ENDING_OFFSET		("ending_offset")

#define CPU_MEMORY_STARTING_ADDRESS	(0x8000)
#define CPU_MEMORY_ENDING_ADDRESS	(0x10000)

/**
 * @brief Load cpu memory parameter from xml file.
 *
 * @param filename		name of file containing parameters for cpu memory structure.
 * @param cpu_memory	cpu memory structure to load.
 * @return 0 if operation succeed, -1 otherwise.
 */
static int load_from_xml(const char *filename, cpu_memory_t *cpu_memory);

/**
 * @brief Get number of slots defined into given xml file.
 *
 * @param filename		name of file containing parameters for cpu memory structure.
 * @return number of slots defined, or -1 if error.
 */
static int get_slot_count(const char *filename);

cpu_memory_t *cpu_init(const char *filename) {
    cpu_memory_t *p_cpu_memory;

    if (NULL == (p_cpu_memory = (cpu_memory_t *) malloc(sizeof(cpu_memory_t)))) {
        LOG_ERROR("cpu_init() failed - cannot allocate memory to initialize a new cpu_memory_t.");
        return NULL;
    }

    if (-1 == load_from_xml(filename, p_cpu_memory)) {
        LOG_ERROR("cpu_init() failed - cannot load cpu_memory data from xml file '%s'.", filename);
        free(p_cpu_memory);
        return NULL;
    }

    return p_cpu_memory;
}

int cpu_modify_slot(cpu_memory_t *cpu_memory, int slot_idx, slot_t slot) {

    if (NULL == cpu_memory) {
        LOG_ERROR("cpu_modify_slot() failed - cpu memory is NULL.");
        return -1;
    }

    if (0 > slot_idx || slot_idx >= cpu_memory->nb_slots) {
        LOG_ERROR("cpu_modify_slot() failed - invalid slot number: %d.", slot_idx);
        return -1;
    }

    cpu_memory->slots[slot_idx] = slot;

    return 0;
}

void cpu_print(cpu_memory_t *p_cpu_memory) {
    int slot_idx;
    int total_size = 0;
    LOG_DEBUG("Content of cpu memory:\n");
    for (slot_idx = 0; slot_idx < p_cpu_memory->nb_slots; slot_idx++) {
        LOG_DEBUG("SLOT #%d: bank #%02X - starting offset: %08X - ending offset: %08X\n", slot_idx + 1,
                p_cpu_memory->slots[slot_idx].bank_number, p_cpu_memory->slots[slot_idx].starting_offset,
                p_cpu_memory->slots[slot_idx].ending_offset);
        total_size += (p_cpu_memory->slots[slot_idx].ending_offset - p_cpu_memory->slots[slot_idx].starting_offset + 1);
    }
    LOG_DEBUG("Total size is $%X.", total_size);
}

long int cpu_get_offset_from_address(const cpu_memory_t *p_cpu_memory, const int cpu_address) {
    if (cpu_address < CPU_MEMORY_STARTING_ADDRESS || cpu_address >= CPU_MEMORY_ENDING_ADDRESS) {
        LOG_ERROR("get_offset_from_cpu_address() failed - invalid cpu address: %04X.", cpu_address);
        return -1;
    }

    int slot_idx;
    int slot_starting_cpu_address = CPU_MEMORY_STARTING_ADDRESS;
    int slot_ending_cpu_address;
    for (slot_idx = 0; slot_idx < p_cpu_memory->nb_slots; slot_idx++) {
        slot_ending_cpu_address = slot_starting_cpu_address
                + (p_cpu_memory->slots[slot_idx].ending_offset - p_cpu_memory->slots[slot_idx].starting_offset);
        if (cpu_address >= slot_starting_cpu_address && cpu_address <= slot_ending_cpu_address) {
            return (p_cpu_memory->slots[slot_idx].starting_offset + (cpu_address - slot_starting_cpu_address));
        }
        slot_starting_cpu_address = slot_ending_cpu_address + 1;
    }
    LOG_ERROR("get_offset_from_cpu_address() failed - cannot convert cpu address %04X - this should never happen.",
            cpu_address);
    return -1;
}

int cpu_get_address_from_offset(const cpu_memory_t *p_cpu_memory, const long int offset) {

    int slot_idx;
    int slot_starting_cpu_address = CPU_MEMORY_STARTING_ADDRESS;
    int slot_ending_cpu_address;
    for (slot_idx = 0; slot_idx < p_cpu_memory->nb_slots; slot_idx++) {
        slot_ending_cpu_address = slot_starting_cpu_address
                + (p_cpu_memory->slots[slot_idx].ending_offset - p_cpu_memory->slots[slot_idx].starting_offset);
        if (offset >= p_cpu_memory->slots[slot_idx].starting_offset
                && offset <= p_cpu_memory->slots[slot_idx].ending_offset) {
            return (slot_starting_cpu_address + (offset - p_cpu_memory->slots[slot_idx].starting_offset));
        }
        slot_starting_cpu_address = slot_ending_cpu_address + 1;
    }
    LOG_ERROR("get_cpu_address_from_offset() failed - cannot convert offset %08lX.", offset);
    return -1;
}

void cpu_uinit(cpu_memory_t *p_cpu_memory) {
    if (!p_cpu_memory) {
        LOG_ERROR("cpu_init() failed - cpu memory is NULL.");
        return;
    }

    free(p_cpu_memory->slots);
    free(p_cpu_memory);
}

/**************************** Private functions *******************************/
static int load_from_xml(const char *filename, cpu_memory_t *p_cpu_memory) {
    int slot_count;
    if (-1 == (slot_count = get_slot_count(filename))) {
        LOG_ERROR("load_from_xml() failed - cannot get slot count from xml file '%s'.", filename);
        return -1;
    }

    if (NULL == (p_cpu_memory->slots = (slot_t *) malloc(slot_count * sizeof(slot_t)))) {
        LOG_ERROR("load_from_xml() failed - cannot allocate memory to initialize %d slot_t.", slot_count);
        return -1;
    }

    xmlTextReaderPtr reader;
    int ret;
    char *attr_value;
    p_cpu_memory->nb_slots = 0;
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            if (XML_READER_TYPE_ELEMENT == xmlTextReaderNodeType(reader)) {
                unsigned char *element_name = xmlTextReaderLocalName(reader);

                if(!strcmp(XML_SLOT_ELEMENT_NAME, (const char *) element_name)) {

                    /* bank number attribute */
                    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(reader,
                                    (const unsigned char *) XML_ATTR_BANK_NUMBER))) {
                        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s' - setting default value %d.",
                                XML_ATTR_BANK_NUMBER, 0);
                        p_cpu_memory->slots[p_cpu_memory->nb_slots].bank_number = 0;
                    } else {
                        sscanf(attr_value, "%02X", &(p_cpu_memory->slots[p_cpu_memory->nb_slots].bank_number));
                        xmlFree(attr_value);
                    }

                    /* starting offset attribute */
                    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(reader,
                                    (const unsigned char *) XML_ATTR_STARTING_OFFSET))) {
                        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s' - setting default value %d.",
                                XML_ATTR_STARTING_OFFSET, 0);
                        p_cpu_memory->slots[p_cpu_memory->nb_slots].starting_offset = 0;
                    } else {
                        sscanf(attr_value, "%08X", &(p_cpu_memory->slots[p_cpu_memory->nb_slots].starting_offset));
                        xmlFree(attr_value);
                    }

                    /* bank number attribute */
                    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(reader,
                                    (const unsigned char *) XML_ATTR_ENDING_OFFSET))) {
                        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s' - setting default value %d.",
                                XML_ATTR_ENDING_OFFSET, 0);
                        p_cpu_memory->slots[p_cpu_memory->nb_slots].ending_offset = 0;
                    } else {
                        sscanf(attr_value, "%08X", &(p_cpu_memory->slots[p_cpu_memory->nb_slots].ending_offset));
                        xmlFree(attr_value);
                    }

                    p_cpu_memory->nb_slots++;
                }
                xmlFree(element_name);
            }
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
    } else {
        LOG_ERROR("get_slot_count() failed - cannot open xml file '%s'.", filename);
        goto error;
    }
    xmlCleanupParser();
    xmlMemoryDump();

    return 0;

    error: xmlCleanupParser();
    xmlMemoryDump();
    return -1;
}
/******************************************************************************/
static int get_slot_count(const char *filename) {

    xmlTextReaderPtr reader;
    int ret;
    int slot_count = 0;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            if (XML_READER_TYPE_ELEMENT == xmlTextReaderNodeType(reader)) {
                unsigned char *element_name = xmlTextReaderLocalName(reader);
                if(!strcmp(XML_SLOT_ELEMENT_NAME, (const char *) element_name)) {
                    slot_count++;
                }
                xmlFree(element_name);
            }
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
    } else {
        LOG_ERROR("get_slot_count() failed - cannot open xml file '%s'.", filename);
        goto error;
    }
    xmlCleanupParser();
    xmlMemoryDump();

    return slot_count;

error:
    xmlCleanupParser();
    xmlMemoryDump();
    return -1;
}
/******************************************************************************/
