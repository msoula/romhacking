/*
 * xml_playlet_writer.c
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#include "xml_playlet_reader.h"

#include <assert.h>
#include <string.h>
#include <libxml/xmlreader.h>

#include "../utils/logger.h"

#define XML_ENCODING               ("UTF-8")

#define XML_XSI_ATTR_PREFIX        ("xmlns")
#define XML_XSI_ATTR               ("xsi")
#define XML_XSI_ATTR_CONTENT       ("http://www.w3.org/2001/XMLSchema-instance")

#define XML_SC_LOC_ATTR_PREFIX     ("xsi")
#define XML_SC_LOC_ATTR            ("schemaLocation")
#define XML_SC_LOC_ATTR_CONTENT    ("http://terminus.romhack.net/XmlScriptEditor_playlet ../resources/xsd/dump_schema.xsd")

#define XML_XSE_S_ATTR_PREFIX     ("xmlns")
#define XML_XSE_S_ATTR            ("xse_s")
#define XML_XSE_S_ATTR_CONTENT    ("http://terminus.romhack.net/XmlScriptEditor_playlet")

#define XML_ELE_PREFIX            ("xse_s")
#define XML_DUMP_ELE              ("dump")
#define XML_PLAYLET_ELE           ("playlet")
#define XML_ANIMATION_ELE         ("animation")
#define XML_SHARED_ANIMATION_ELE  ("shared_animation")
#define XML_BANK_ATTR             ("bank")
#define XML_CPU_ADDR_ATTR         ("cpu_address")
#define XML_LENGTH_ATTR           ("length")
#define XML_DATA_ATTR             ("data")

static xmlTextReaderPtr _p_xml_reader = NULL;

static int readBankNumberAttr(u_int8_t *bank_nb);
static int readAddressAttr(u_int16_t *address);

static int readAnimationLengthAttr(u_int16_t *length);
static int readAnimationDataAttr(array_t *data);

static u_int8_t char2hex(char c);

int xml_playlet_reader_init(const char *filename) {

    if (NULL != _p_xml_reader) {
        LOG_ERROR("xml_playlet_reader_init() failed - xml reader is already initialized.");
        return 1;
    }

    /* Create a new XmlWriter for uri, with no compression. */
    if (NULL == (_p_xml_reader = xmlReaderForFile(filename, NULL, 0))) {
        LOG_ERROR("xml_playlet_reader_init() failed - cannot create xml read for file '%s'.", filename);
        return 1;
    }

    return 0;
}
void xml_playlet_reader_uinit() {
    assert(_p_xml_reader);

    xmlFreeTextReader(_p_xml_reader);
    xmlCleanupParser();
    xmlMemoryDump();
}

int xml_playlet_reader_parse(ptrlist_t *shared_animations, ptrlist_t *playlets) {
    assert(_p_xml_reader);

    int ret;
    playlet_t     *playlet                = NULL;
    int            playlet_count          = 0;
    shared_anim_t *shared_animation       = NULL;
    int            shared_animation_count = 0;

    /* init lists */
    ptrlist_init(shared_animations);
    ptrlist_init(playlets);

    ret = xmlTextReaderRead(_p_xml_reader);
    while (ret == 1) {
        if (XML_READER_TYPE_ELEMENT == xmlTextReaderNodeType(_p_xml_reader)) {
            unsigned char *element_name = xmlTextReaderLocalName(_p_xml_reader);

            if (!strcmp(XML_PLAYLET_ELE, (const char *) element_name)) {

                u_int8_t bank_nb;
                u_int16_t address;

                /* read playlet element attributes */
                readBankNumberAttr(&bank_nb);
                readAddressAttr(&address);

                /* init new playlet and add it into list */
                playlet = playlet_init(playlet_count++, bank_nb, address);
                ptrlist_add(playlets, playlet);

            } else if (!strcmp(XML_SHARED_ANIMATION_ELE, (const char *) element_name)) {

                u_int16_t address;

                /* read shared animation element attributes */
                readAddressAttr(&address);

                /* init new playlet and add it into list */
                shared_animation = shared_anim_init(shared_animation_count++, address);

                /* read shared animation data attribute */
                readAnimationDataAttr(&(shared_animation->data));

                ptrlist_add(shared_animations, shared_animation);

            } else if (!strcmp(XML_ANIMATION_ELE, (const char *) element_name)) {

                ptrlist_t *list = NULL;
                if(NULL != playlet)
                    list = &(playlet->anims);
                else if(NULL != shared_animation)
                    list = &(shared_animation->anims);

                if(NULL != list) {
                    anim_t *anim    = anim_init();

                    /* read playlet element attributes */
                    readAnimationLengthAttr(&(anim->length));
                    readAnimationDataAttr(&(anim->data));
                    ptrlist_add(list,anim);
                }

            }

            xmlFree(element_name);
        }
        else if (XML_READER_TYPE_END_ELEMENT == xmlTextReaderNodeType(_p_xml_reader)) {
            unsigned char *element_name = xmlTextReaderLocalName(_p_xml_reader);

            if (!strcmp(XML_PLAYLET_ELE, (const char *) element_name)) {
                playlet = NULL;
            } else if (!strcmp(XML_SHARED_ANIMATION_ELE, (const char *) element_name)) {
                shared_animation = NULL;
            }

            xmlFree(element_name);
        }

        ret = xmlTextReaderRead(_p_xml_reader);
    }

    return 0;
}

/******************************************************************************/

int readBankNumberAttr(u_int8_t *bank_nb) {
    int   res;
    char *attr_value;

    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(_p_xml_reader, (const unsigned char *) XML_BANK_ATTR))) {
        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s' - setting default value %d.", XML_BANK_ATTR, 0);
        res = 1;
    } else {
        unsigned int tmp;
        sscanf(attr_value, "%2X", &tmp);
        *bank_nb = tmp;
        xmlFree(attr_value);
        res = 0;
    }

    return res;
}

/******************************************************************************/

int readAddressAttr(u_int16_t *address) {
    int   res;
    char *attr_value;

    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(_p_xml_reader, (const unsigned char *) XML_CPU_ADDR_ATTR))) {
        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s' - setting default value %d.", XML_CPU_ADDR_ATTR, 0);
        res = 1;
    } else {
        unsigned int tmp;
        sscanf(attr_value, "%4X", &tmp);
        *address = tmp;
        xmlFree(attr_value);
        res = 0;
    }

    return res;
}

/******************************************************************************/

int readAnimationLengthAttr(u_int16_t *length) {
    int   res;
    char *attr_value;

    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(_p_xml_reader,
                    (const unsigned char *) XML_LENGTH_ATTR))) {
        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s' - setting default value %d.",
                XML_LENGTH_ATTR, 0);
        res = 1;
    } else {
        unsigned int tmp;
        sscanf(attr_value, "%4X", &tmp);
        *length = tmp;
        xmlFree(attr_value);
        res = 1;
    }

    return res;
}

/******************************************************************************/

int readAnimationDataAttr(array_t *data) {
    int   res;
    char *attr_value;

    /* length number attribute */
    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(_p_xml_reader,
                    (const unsigned char *) XML_DATA_ATTR))) {
        LOG_ERROR("load_from_xml() - cannot get value of attribute '%s'.", XML_DATA_ATTR);
        res = 1;
    } else {
        /* parse value */
        int pos = 0;
        while ('\0' != attr_value[pos*2]) {
            u_int8_t byte;

            /* read two chars */
            char c1 = attr_value[pos* 2];
            char c2 = attr_value[pos*2 + 1];

            byte = char2hex(c1) * 0x10 + char2hex(c2);
            array_add(data, &byte);

            pos++;
        }
        xmlFree(attr_value);
        res = 0;
    }

    return res;
}

/******************************************************************************/

u_int8_t char2hex(char c) {
    if('A' ==  c || 'a' == c)
        return 0xA;
    else if('B' ==  c || 'b' == c)
        return 0xB;
    else if('C' ==  c || 'c' == c)
        return 0xC;
    else if('D' ==  c || 'd' == c)
        return 0xD;
    else if('E' ==  c || 'e' == c)
        return 0xE;
    else if('F' ==  c || 'f' == c)
        return 0xF;
    else
        return (u_int8_t) c - '0';
}
