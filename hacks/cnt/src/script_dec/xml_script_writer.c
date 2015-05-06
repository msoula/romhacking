/*
 * xml_script_writer.c
 *
 *  Created on: 10 sept. 2011
 *      Author: rid
 */

#include "xml_script_writer.h"

#include <assert.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <stdio.h>

#include "../utils/logger.h"

#define XML_ENCODING			("UTF-8")

#define XML_XSI_ATTR_PREFIX		("xmlns")
#define XML_XSI_ATTR			("xsi")
#define XML_XSI_ATTR_CONTENT	("http://www.w3.org/2001/XMLSchema-instance")

#define XML_SC_LOC_ATTR_PREFIX	("xsi")
#define XML_SC_LOC_ATTR			("schemaLocation")
#define XML_SC_LOC_ATTR_CONTENT	("http://terminus.romhack.net/XmlScriptEditor_script ../resources/xsd/dump_schema.xsd")

#define XML_XSE_S_ATTR_PREFIX	("xmlns")
#define XML_XSE_S_ATTR			("xse_s")
#define XML_XSE_S_ATTR_CONTENT	("http://terminus.romhack.net/XmlScriptEditor_script")

#define XML_ELE_PREFIX			("xse_s")
#define	XML_DUMP_ELE			("dump")
#define XML_POINTER_ELE			("pointer")
#define XML_CPU_ADDR_ATTR		("cpu_address")
#define XML_TRANSLATED_ATTR		("translated")
#define XML_DONE_ATTR			("done")
#define XML_ATTR_VAL_FALSE		("false")


static xmlTextWriterPtr _p_xml_writer = NULL;

int xml_script_writer_init(const char *filename) {
	int rc;

	/* Create a new XmlWriter for uri, with no compression. */
    if(NULL == (_p_xml_writer = xmlNewTextWriterFilename(filename, 0))) {
        LOG_ERROR("xml_script_writer_init() failed - cannot create xml writer for uri '%s'.", filename);
        return -1;
    }

    /* pretty print */
    xmlTextWriterSetIndent(_p_xml_writer, 1);

    /* Start the document with the xml default for the version,
     * encoding and the default for the standalone declaration. */
    if (0 > (rc = xmlTextWriterStartDocument(_p_xml_writer, NULL, XML_ENCODING, NULL))) {
    	LOG_ERROR("xml_script_writer_init() failed - cannot start %s document.", XML_ENCODING);
        xmlFreeTextWriter(_p_xml_writer);
        return -1;
    }
    /* Start an element named "EXAMPLE". Since thist is the first
     * element, this will be the root element of the document. */
    if (0 > (rc = xmlTextWriterStartElementNS(_p_xml_writer, (const xmlChar *) XML_ELE_PREFIX, (const xmlChar *) XML_DUMP_ELE, NULL))) {
    	LOG_ERROR("xml_script_writer_init() failed - cannot start '%s:%s' element.", XML_ELE_PREFIX, XML_DUMP_ELE);
        xmlFreeTextWriter(_p_xml_writer);
        return -1;
    }

    /* Add an namespace related attribute. */
	if(0 > xmlTextWriterWriteAttributeNS(_p_xml_writer, (const xmlChar *) XML_XSI_ATTR_PREFIX, (const xmlChar *) XML_XSI_ATTR, NULL, (const xmlChar *) XML_XSI_ATTR_CONTENT)) {
		LOG_ERROR("xml_script_writer_init() failed - cannot write '%s' attribute with value '%s'.", XML_XSI_ATTR, XML_XSI_ATTR_CONTENT);
		return -1;
	}
	if(0 > xmlTextWriterWriteAttributeNS(_p_xml_writer, (const xmlChar *) XML_SC_LOC_ATTR_PREFIX, (const xmlChar *) XML_SC_LOC_ATTR, NULL, (const xmlChar *) XML_SC_LOC_ATTR_CONTENT)) {
		LOG_ERROR("xml_script_writer_init() failed - cannot write '%s' attribute with value '%s'.", XML_SC_LOC_ATTR, XML_SC_LOC_ATTR_CONTENT);
		return -1;
	}
	if(0 > xmlTextWriterWriteAttributeNS(_p_xml_writer, (const xmlChar *) XML_XSE_S_ATTR_PREFIX, (const xmlChar *) XML_XSE_S_ATTR, NULL, (const xmlChar *) XML_XSE_S_ATTR_CONTENT)) {
		LOG_ERROR("xml_script_writer_init() failed - cannot write '%s' attribute with value '%s'.", XML_XSE_S_ATTR, XML_XSE_S_ATTR_CONTENT);
		return -1;
	}
    return 0;
}

int xml_script_writer_add_pointer_element(int cpu_address, const char *text) {
	assert(_p_xml_writer);

	char attrValue[BUFSIZ];
	/* Start an element named "ENTRY" as child of ENTRIES. */
	if (0 > xmlTextWriterStartElementNS(_p_xml_writer, (const xmlChar *) XML_ELE_PREFIX, (const xmlChar *) XML_POINTER_ELE, NULL)) {
    	LOG_ERROR("xml_script_writer_init() failed - cannot start '%s:%s' element.", XML_ELE_PREFIX, XML_POINTER_ELE);
    	return -1;
	}

	/* Add an attribute with name "cpu_addr". */
    sprintf(attrValue, "%04X", cpu_address);
    if(0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_CPU_ADDR_ATTR, (const xmlChar *) attrValue)) {
        LOG_ERROR("xml_script_writer_init() failed - cannot write '%s' attribute with value '%s'.", XML_CPU_ADDR_ATTR, attrValue);
        return -1;
    }

	/* Add an attribute with name "translated" and value "false". */
    if(0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_TRANSLATED_ATTR, (const xmlChar *) XML_ATTR_VAL_FALSE)) {
        LOG_ERROR("xml_script_writer_init() failed - cannot write '%s' attribute with value '%s'.", XML_TRANSLATED_ATTR, XML_ATTR_VAL_FALSE);
        return -1;
    }
	/* Add an attribute with name "done" and value "false". */
    if(0 > xmlTextWriterWriteAttribute(_p_xml_writer, (const xmlChar *) XML_DONE_ATTR, (const xmlChar *) XML_ATTR_VAL_FALSE)) {
        LOG_ERROR("xml_script_writer_init() failed - cannot write '%s' attribute with value '%s'.", XML_DONE_ATTR, XML_ATTR_VAL_FALSE);
        return -1;
    }

    /* writing element content */
    if(0 > xmlTextWriterWriteCDATA(_p_xml_writer, (const xmlChar *) text)) {
        LOG_ERROR("xml_script_writer_init() failed - cannot write pointer content '%s'.", text);
        return -1;
    }

    /* Close the element named ENTRY. */
    if(0 > xmlTextWriterEndElement(_p_xml_writer)) {
        LOG_ERROR("xml_script_writer_init() failed - cannot end '%s:%s' element.", XML_ELE_PREFIX, XML_POINTER_ELE);
        return -1;
    }

    return 0;
}

void xml_script_writer_to_file() {
	assert(_p_xml_writer);

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    if(0 > xmlTextWriterEndDocument(_p_xml_writer)) {
        LOG_ERROR("xml_script_writer_to_file() failed - cannot end document and flush to output.");
        return;
    }
}

void xml_script_writer_uinit() {
	assert(_p_xml_writer);

    xmlFreeTextWriter(_p_xml_writer);
}
