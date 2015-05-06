/*
 * table.c
 *
 *  Created on: 25 juil. 2011
 *      Author: rid
 */

#include "table.h"

#include <assert.h>
#include <errno.h>
#include <libxml/xmlreader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/logger.h"
#include "../utils/spc_utf8.h"
#include "../utils/util.h"

#define XML_ENTRY_ELEMENT_NAME		("character")
#define XML_ENTRY_HEX_VALUE_ATTR	("hexValue")

/**
 * @fn
 * @brief Get xml table entry count.
 *
 * @param filename	xml filename.
 * @return xml table entry count, or -1 if error.
 */
static table_entry_t *init_table_entries(const char *filename);

static int parse_xml_table(table_t *table, const char *filename);

/**
 * \fn int init_table(int length)
 * \brief Initialize table with a given number of entries.
 *
 * \param length table length.
 * \return the initialized table length, or -1 on error.
 */
int init_table(table_t *p_table, int length);

void table_free(table_t *p_table) {

    if (!p_table) {
        LOG_DEBUG("table_free() failed - table is NULL.");
        return;
    }

    while (p_table->length > 0) {
        if (p_table->entries[p_table->length - 1].value)
            free(p_table->entries[p_table->length - 1].value), p_table->entries[p_table->length - 1].value = NULL;
        --p_table->length;
    }
}

table_t *table_load_from_tbl(const char *filepath) {
    int i, line_number = 0;
    char line[LINE_MAX_SIZE];

    table_t *p_table;
    if (NULL == (p_table = (table_t *) malloc(sizeof(table_t)))) {
        LOG_ERROR("table_load_from_tbl() failed - cannot allocate memory to initialize a new table_t.");
        return NULL;
    }

    /* loading table data from file */
    LOG_DEBUG("table_load_from_tbl() - loading table from tbl file %s.", filepath);
    FILE *pFile;
    if (NULL == (pFile = fopen(filepath, "r"))) {
        LOG_ERROR("table_load_from_tbl() failed - cannot open table file '%s'.", filepath);
        free(p_table);
        return NULL;
    }

    /* counting line number. */
    if (-1 == (line_number = get_line_number(pFile))) {
        LOG_ERROR("table_load_from_tbl() failed - cannot count number of lines in table file '%s'.", filepath);
        free(p_table);
        return NULL;
    }

    /* initializing table */
    if (-1 == init_table(p_table, line_number)) {
        LOG_ERROR("table_load_from_tbl() failed - cannot initialized table with %d entries.", line_number);
        free(p_table);
        return NULL;
    }

    for (i = 0; i < line_number; i++) {
        char *split;

        fgets(line, LINE_MAX_SIZE, pFile);

        /* get index value */
        if (NULL == (split = strtok(line, "="))) {
            LOG_ERROR("table_load_from_tbl() - read line with no '=' char.");
            fgets(line, LINE_MAX_SIZE, pFile);
            break;
        } else
            p_table->entries[i].index = strtol(split, NULL, 16);

        /* get indexed value */
        if (NULL == (split = strtok(NULL, "="))) {
            LOG_ERROR("table_load_from_tbl() - strtok returned NULL but it is impossible (line=%s).", line);
        } else {
            p_table->entries[i].value = u8_convert_to_u8str(trim_ending_jump_line(split));
        }
    }

    if (0 != fclose(pFile)) {
        LOG_ERROR("table_load_from_tbl() - cannot close table file '%s' - errno=%d(%s)\n", filepath, errno,
                strerror(errno));
    }
    return 0;
}

table_t *table_load_from_xml(const char *filepath) {
    /* loading table data from file */
    LOG_DEBUG("table_load_from_xml() - loading table from xml file %s...", filepath);

    table_t *p_table;
    if (NULL == (p_table = (table_t *) malloc(sizeof(table_t)))) {
        LOG_ERROR("table_load_from_xml() failed - cannot allocate memory to initialize a new table_t.");
        return NULL;
    }

    if (-1 == parse_xml_table(p_table, filepath)) {
        LOG_ERROR("table_load_from_xml() failed - cannot parse xml file '%s'.", filepath);
        free(p_table);
        return NULL;
    }
    LOG_DEBUG("table_load_from_xml() - %d entries loaded.", p_table->length);

    return p_table;
}

int table_get_best_value_index(const table_t *table, const u_int32_t *str, u_int8_t *index) {
    assert(table);
    int idx;
    int length = 0;

    for (idx = 0; idx < table->length; idx++) {

        table_entry_t *cur = &(table->entries[idx]);

        if (u8_starts_with(str, cur->value)) {

            if(length < u8_strlen(cur->value)) {
                length = u8_strlen(cur->value);
                *index = cur->index;
            }

        }
    }

    return length;

}

u_int32_t *table_get_entry(const table_t *p_table, const unsigned int hex_value) {
    u_int32_t *matched_entry = NULL;

    if (!p_table) {
        LOG_DEBUG("table_get_entry() - table is NULL.");
        return matched_entry;
    }

    int idx;
    for (idx = 0; matched_entry == NULL && idx < p_table->length; idx++) {
        if (p_table->entries[idx].index == hex_value) {
            matched_entry = p_table->entries[idx].value;
        }
    }
    return matched_entry;
}

void table_print(table_t *p_table) {
    int idx;

    if (!p_table) {
        LOG_DEBUG("table_print() - table is NULL.");
        return;
    }

    for (idx = 0; idx < p_table->length; idx++) {
        LOG_DEBUG("%04X=%s\n", p_table->entries[idx].index, u8_convert_to_str(p_table->entries[idx].value));
    }
}

/************************** Private functions *********************************/
int init_table(table_t *p_table, int length) {
    if (length < 0) {
        LOG_ERROR("init_table() failed - invalid length parameter %d.", length);
        return -1;
    }

    if (!p_table) {
        LOG_ERROR("init_table() failed - table is NULL.");
        return -1;
    }

    if (NULL == (p_table->entries = (table_entry_t *) calloc(length, sizeof(table_entry_t)))) {
        LOG_ERROR("cannot allocate memory to initialize %d table_entry_t.", length);
        return -1;
    }

    return length;
}
/******************************************************************************/
static table_entry_t *init_table_entries(const char *filename) {

    xmlTextReaderPtr reader;
    int ret;
    int entry_count = 0;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            if (XML_READER_TYPE_ELEMENT == xmlTextReaderNodeType(reader)
                    && !strcmp(XML_ENTRY_ELEMENT_NAME, (const char *) xmlTextReaderLocalName(reader))) {
                entry_count++;
            }
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            LOG_ERROR("%s : failed to parse\n", filename);
            goto error;
        }
    } else {
        LOG_ERROR("Unable to open %s\n", filename);
        goto error;
    }
    xmlCleanupParser();
    xmlMemoryDump();

    table_entry_t *p_entries = NULL;
    if (NULL == (p_entries = (table_entry_t *) malloc(entry_count * sizeof(table_entry_t)))) {
        LOG_ERROR("init_table_entries() failed - cannot allocate memory to initialize %d table entries.", entry_count);
        return NULL;
    }
    return p_entries;

    error: xmlCleanupParser();
    xmlMemoryDump();
    return NULL;
}
/******************************************************************************/
static int parse_xml_table(table_t *p_table, const char *filename) {

    table_entry_t *p_entries;
    if (NULL == (p_entries = init_table_entries(filename))) {
        LOG_ERROR("parse_xml_table() failed - cannot initialize table entry list from xml.");
        return -1;
    }

    xmlTextReaderPtr reader;
    int ret;
    int entry_idx = 0;
    int is_character_element = 0;
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            if (XML_READER_TYPE_ELEMENT == xmlTextReaderNodeType(reader)
                    && !strcmp(XML_ENTRY_ELEMENT_NAME, (const char *) xmlTextReaderLocalName(reader))) {
                /* setting hexValue value. */
                sscanf((char *) xmlTextReaderGetAttribute(reader, (const unsigned char *) XML_ENTRY_HEX_VALUE_ATTR),
                        "%02X", &(p_entries[entry_idx].index));
                is_character_element = 1;
            } else if (XML_READER_TYPE_END_ELEMENT == xmlTextReaderNodeType(reader)
                    && !strcmp(XML_ENTRY_ELEMENT_NAME, (const char *) xmlTextReaderLocalName(reader))) {
                is_character_element = 0;
                entry_idx++;
            } else if (XML_READER_TYPE_TEXT && is_character_element) {
                const char *entry_value = (const char *) xmlTextReaderValue(reader);
                p_entries[entry_idx].value = u8_convert_to_u8str(entry_value);
            }
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            LOG_ERROR("%s : failed to parse\n", filename);
            goto error;
        }
    } else {
        LOG_ERROR("Unable to open %s\n", filename);
        goto error;
    }
    xmlCleanupParser();
    xmlMemoryDump();

    p_table->entries = p_entries;
    p_table->length = entry_idx;

    return 0;

    error: xmlCleanupParser();
    xmlMemoryDump();
    if (p_entries)
        free(p_entries);
    return -1;
}
/******************************************************************************/
