/*
 * xml_script_reader.c
 *
 *  Created on: 10 déc. 2014
 *      Author: rid
 */

#include "xml_script_reader.h"

#include <assert.h>
#include <string.h>
#include <libxml/xmlreader.h>

#include "utils/logger.h"
#include "utils/spc_utf8.h"

#define XML_ENCODING            ("UTF-8")

#define XML_XSI_ATTR_PREFIX     ("xmlns")
#define XML_XSI_ATTR            ("xsi")
#define XML_XSI_ATTR_CONTENT    ("http://www.w3.org/2001/XMLSchema-instance")

#define XML_SC_LOC_ATTR_PREFIX  ("xsi")
#define XML_SC_LOC_ATTR         ("schemaLocation")
#define XML_SC_LOC_ATTR_CONTENT ("http://terminus.romhack.net/XmlScriptEditor_script ../resources/xsd/dump_schema.xsd")

#define XML_XSE_S_ATTR_PREFIX   ("xmlns")
#define XML_XSE_S_ATTR          ("xse_s")
#define XML_XSE_S_ATTR_CONTENT  ("http://terminus.romhack.net/XmlScriptEditor_script")

#define XML_ELE_PREFIX          ("xse_s")
#define XML_DUMP_ELE            ("dump")
#define XML_POINTER_ELE         ("pointer")
#define XML_CPU_ADDR_ATTR       ("cpu_address")
#define XML_TRANSLATED_ATTR     ("translated")
#define XML_DONE_ATTR           ("done")
#define XML_ATTR_VAL_FALSE      ("false")

static xmlTextReaderPtr _p_xml_reader = NULL;

static int readPointerAddressAttr(u_int16_t *address);
static int readPointerCData(u_int32_t **cdata);

int xml_script_reader_init(const char *filename) {

    if (NULL != _p_xml_reader) {
        LOG_ERROR("xml_script_reader_init() failed - xml reader is already initialized.");
        return 1;
    }

    /* Create a new XmlWriter for uri, with no compression. */
    if (NULL == (_p_xml_reader = xmlReaderForFile(filename, NULL, 0))) {
        LOG_ERROR("xml_script_reader_init() failed - cannot create xml read for file '%s'.", filename);
        return 1;
    }

    return 0;
}
void xml_script_reader_uinit() {
    assert(_p_xml_reader);

    xmlFreeTextReader(_p_xml_reader);
    xmlCleanupParser();
    xmlMemoryDump();
}

int xml_script_reader_parse(script_t *script) {
    assert(_p_xml_reader);

    /* init list of playlets */
    int idx = 0;
    text_t *cur_text = NULL;
    ptrlist_init(&(script->texts));

    int            ret;
    int            element_type = XML_READER_TYPE_NONE;
    unsigned char *element_name = '\0';


    ret = xmlTextReaderRead(_p_xml_reader);
    while (ret == 1) {
        element_type = xmlTextReaderNodeType(_p_xml_reader);

        if (XML_READER_TYPE_ELEMENT == element_type) {
            element_name = xmlTextReaderLocalName(_p_xml_reader);

            if(!strcmp(XML_POINTER_ELE, (const char *) element_name)) {

                /* starting new text */
                cur_text = text_init();

                /* setting pointer index value. */
                cur_text->pointer_idx = idx++;

                /* setting cpu address value. */
                readPointerAddressAttr(&(cur_text->cpu_address));
            }

            xmlFree(element_name);
        }
        else if (XML_READER_TYPE_END_ELEMENT == element_type) {
            element_name = xmlTextReaderLocalName(_p_xml_reader);

            if(!strcmp(XML_POINTER_ELE, (const char *) element_name)) {
                /* end of text*/
                if(NULL != cur_text)
                    ptrlist_add(&(script->texts), cur_text);
                cur_text = NULL;
            }

            xmlFree(element_name);
        }
        else if (XML_READER_TYPE_CDATA == element_type && NULL != cur_text) {

            readPointerCData(&(cur_text->u8srt));

        }

        ret = xmlTextReaderRead(_p_xml_reader);
    }

    return 0;

}

/******************************************************************************/

int readPointerAddressAttr(u_int16_t *address) {
    int   res;
    char *attr_value;

    if (NULL == (attr_value = (char *) xmlTextReaderGetAttribute(_p_xml_reader, (const unsigned char *) XML_CPU_ADDR_ATTR))) {
        LOG_ERROR("readPointerAddressAttr() - cannot get value of attribute '%s' - setting default value %d.", XML_CPU_ADDR_ATTR, 0);
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

int readPointerCData(u_int32_t **u8str) {
    int res;

    const unsigned char *cdata_content = xmlTextReaderConstValue(_p_xml_reader);
    if (NULL != cdata_content) {
        *(u8str) = u8_convert_to_u8str((const char *) cdata_content);
    } else {
        LOG_DEBUG("WARNING: CDATA node cannot be read.");
        res = 1;
    }

    return res;
}
