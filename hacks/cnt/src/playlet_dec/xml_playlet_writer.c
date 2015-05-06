/*
 * xml_playlet_writer.c
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#include "xml_playlet_writer.h"

#include <assert.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <stdio.h>

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
#define XML_INDEX_ATTR            ("index")
#define XML_CPU_ADDR_ATTR         ("cpu_address")
#define XML_BANK_ATTR             ("bank")
#define XML_LENGTH_ATTR           ("length")
#define XML_DATA_ATTR             ("data")
#define XML_ISTEXT_ATTR           ("is_text")

static xmlTextWriterPtr _p_xml_writer = NULL;

int xml_playlet_writer_init(const char *filename) {
    int rc;


    /* Create a new XmlWriter for uri, with no compression. */
    if (NULL == (_p_xml_writer = xmlNewTextWriterFilename(filename, 0))) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot create xml writer for uri '%s'.", filename);
        return -1;
    }

    /* pretty print */
    xmlTextWriterSetIndent(_p_xml_writer, 1);

    /* Start the document with the xml default for the version,
     * encoding and the default for the standalone declaration. */
    if (0 > (rc = xmlTextWriterStartDocument(_p_xml_writer, NULL, XML_ENCODING, NULL))) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot start %s document.", XML_ENCODING);
        xmlFreeTextWriter(_p_xml_writer);
        return -1;
    }
    /* Start an element named "EXAMPLE". Since thist is the first
     * element, this will be the root element of the document. */
    if (0 > (rc = xmlTextWriterStartElementNS(_p_xml_writer,
            (const xmlChar *) XML_ELE_PREFIX, (const xmlChar *) XML_DUMP_ELE, NULL))) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot start '%s:%s' element.", XML_ELE_PREFIX, XML_DUMP_ELE);
        xmlFreeTextWriter(_p_xml_writer);
        return -1;
    }

    /* Add an namespace related attribute. */
    if (0 > xmlTextWriterWriteAttributeNS(_p_xml_writer, (const xmlChar *) XML_XSI_ATTR_PREFIX,
            (const xmlChar *) XML_XSI_ATTR, NULL, (const xmlChar *) XML_XSI_ATTR_CONTENT)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_XSI_ATTR, XML_XSI_ATTR_CONTENT);
        return -1;
    }
    if (0 > xmlTextWriterWriteAttributeNS(_p_xml_writer, (const xmlChar *) XML_SC_LOC_ATTR_PREFIX,
            (const xmlChar *) XML_SC_LOC_ATTR, NULL, (const xmlChar *) XML_SC_LOC_ATTR_CONTENT)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_SC_LOC_ATTR, XML_SC_LOC_ATTR_CONTENT);
        return -1;
    }
    if (0 > xmlTextWriterWriteAttributeNS(_p_xml_writer, (const xmlChar *) XML_XSE_S_ATTR_PREFIX,
            (const xmlChar *) XML_XSE_S_ATTR, NULL, (const xmlChar *) XML_XSE_S_ATTR_CONTENT)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_XSE_S_ATTR, XML_XSE_S_ATTR_CONTENT);
        return -1;
    }
    return 0;
}

int xml_playlet_writer_add_playlet(playlet_t *playlet) {
    assert(_p_xml_writer);

    int idx;
    char attrValue[BUFSIZ];

    /* Start an element named "PLAYLET". */
    if (0 > xmlTextWriterStartElementNS(_p_xml_writer, (const xmlChar *) XML_ELE_PREFIX,
            (const xmlChar *) XML_PLAYLET_ELE, NULL)) {
        LOG_ERROR("xml_playlet_writer_start_playlet_element() failed - cannot start '%s:%s' element.",
                XML_ELE_PREFIX, XML_PLAYLET_ELE);
        return -1;
    }

    /* Add an attribute with name "index". */
    sprintf(attrValue, "%02X", playlet->index);
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_INDEX_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_INDEX_ATTR, attrValue);
        return -1;
    }

    /* Add an attribute with name "bank". */
    sprintf(attrValue, "%02X", playlet->bank);
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_BANK_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_BANK_ATTR, attrValue);
        return -1;
    }

    /* Add an attribute with name "cpu_addr". */
    sprintf(attrValue, "%04X", playlet->address);
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_CPU_ADDR_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_CPU_ADDR_ATTR, attrValue);
        return -1;
    }

    /* add children */
    for(idx = 0; idx < ptrlist_count(&(playlet->anims)); idx++) {
        xml_playlet_writer_add_animation(ptrlist_get(&(playlet->anims), idx));
    }

    /* Close the element named PLAYLET. */
    if (0 > xmlTextWriterEndElement(_p_xml_writer)) {
        LOG_ERROR("xml_playlet_writer_stop_playlet_element() failed - cannot end '%s:%s' element.",
                XML_ELE_PREFIX, XML_PLAYLET_ELE);
        return -1;
    }

    return 0;
}

int xml_playlet_writer_add_shared_animation(shared_anim_t *shared_animation) {
    assert(_p_xml_writer);

    int idx;
    char attrValue[BUFSIZ];

    /* Start an element named "PLAYLET". */
    if (0 > xmlTextWriterStartElementNS(_p_xml_writer, (const xmlChar *) XML_ELE_PREFIX,
            (const xmlChar *) XML_SHARED_ANIMATION_ELE, NULL)) {
        LOG_ERROR("xml_playlet_writer_start_playlet_element() failed - cannot start '%s:%s' element.",
                XML_ELE_PREFIX, XML_SHARED_ANIMATION_ELE);
        return -1;
    }

    /* Add an attribute with name "index". */
    sprintf(attrValue, "%02X", shared_animation->index);
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_INDEX_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_INDEX_ATTR, attrValue);
        return -1;
    }

    /* Add an attribute with name "cpu_addr". */
    sprintf(attrValue, "%04X", shared_animation->address);
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_CPU_ADDR_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_CPU_ADDR_ATTR, attrValue);
        return -1;
    }

    /* Add an attribute with name "data". */

    /* prepare data attribute value */
    for(idx = 0; idx < array_count(&(shared_animation->data)); idx++) {
        sprintf(&attrValue[idx*2], "%02X", *((u_int8_t *) array_get(&(shared_animation->data), idx)));
    }
    attrValue[idx*2] = '\0';

    /* Add an attribute with name "data". */
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_DATA_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_DATA_ATTR, attrValue);
        return -1;
    }

    /* add animations children */
    for(idx = 0; idx < ptrlist_count(&(shared_animation->anims)); idx++) {
        xml_playlet_writer_add_animation(ptrlist_get(&(shared_animation->anims), idx));
    }

    /* Close the element named PLAYLET. */
    if (0 > xmlTextWriterEndElement(_p_xml_writer)) {
        LOG_ERROR("xml_playlet_writer_stop_playlet_element() failed - cannot end '%s:%s' element.",
                XML_ELE_PREFIX, XML_PLAYLET_ELE);
        return -1;
    }

    return 0;

}

int xml_playlet_writer_add_animation(anim_t *animation) {

    int idx;
    char attrValue[BUFSIZ];

    /* Start an element named "ANIMATION". */
    if (0 > xmlTextWriterStartElementNS(_p_xml_writer, (const xmlChar *) XML_ELE_PREFIX,
            (const xmlChar *) XML_ANIMATION_ELE, NULL)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot start '%s:%s' element.", XML_ELE_PREFIX, XML_ANIMATION_ELE);
        return -1;
    }

    /* Add an attribute with name "length" */
    sprintf(attrValue, "%04X", animation->length);
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_LENGTH_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_LENGTH_ATTR, attrValue);
        return -1;
    }

    /* prepare data attribute value */
    for(idx = 0; idx < array_count(&(animation->data)); idx++) {
        sprintf(&attrValue[idx*2], "%02X", *((u_int8_t *) array_get(&(animation->data), idx)));
    }
    attrValue[idx*2] = '\0';

    /* Add an attribute with name "data". */
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_DATA_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_DATA_ATTR, attrValue);
        return -1;
    }

    /* Add an attribute with name "is_text". */
    if (0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_ISTEXT_ATTR, (const xmlChar *) "no")) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot write '%s' attribute with value '%s'.",
                XML_DATA_ATTR, attrValue);
        return -1;
    }

    /* Close the element named ANIMATION. */
    if (0 > xmlTextWriterEndElement(_p_xml_writer)) {
        LOG_ERROR("xml_playlet_writer_init() failed - cannot end '%s:%s' element.", XML_ELE_PREFIX, XML_ANIMATION_ELE);
        return -1;
    }

    return 0;
}

void xml_playlet_writer_to_file() {
    assert(_p_xml_writer);

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    if (0 > xmlTextWriterEndDocument(_p_xml_writer)) {
        LOG_ERROR("xml_playlet_writer_to_file() failed - cannot end document and flush to output.");
        return;
    }
}

void xml_playlet_writer_uinit() {
    assert(_p_xml_writer);

    xmlFreeTextWriter(_p_xml_writer);
}
