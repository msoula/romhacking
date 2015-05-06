/*
 * XmlScriptReader.cpp
 *
 *  Created on: 1 nov. 2010
 *      Author: rid
 */

#define DEBUG_XMLSCRIPTREADER	// Enable debug mode
//#undef DEBUG_XMLSCRIPTREADER	// Disable debug mode

#define TRACE_XMLSCRIPTREADER	// Enable trace mode
#undef TRACE_XMLSCRIPTREADER	// Disable trace mode

#include "XmlScriptReader.h"

#include <QtCore/QFile>

#include "XmlScriptHandler.h"
#include "XmlValidator.h"

XmlScriptReader* XmlScriptReader::_pInstance = NULL;

/**
 * @brief Constructor of an instance of XmlScriptReader.
 */
XmlScriptReader::XmlScriptReader() {
#ifdef DEBUG_XMLSCRIPTREADER
	{
		std::stringstream log;
		log << "Constructing a new XmlScriptReader object...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Destructor of this instance of XmlScriptReader.
 */
XmlScriptReader::~XmlScriptReader() {
#ifdef DEBUG_XMLSCRIPTREADER
	{
		std::stringstream log;
		log << "Destroying this XmlScriptReader...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * Get an instance of this XmlScriptReader.
 */
XmlScriptReader* XmlScriptReader::getInstance() {
#ifdef TRACE_XMLSCRIPTREADER
	{
		std::stringstream log;
		log << "XmlScriptReader::getInstance()";
		LOG_INFO(log.str());
	}
#endif
	if(!_pInstance) {
		_pInstance = new XmlScriptReader;
	}
	return _pInstance;
}

/**
 * @brief Loads a Script from an xml script.
 *
 * @param pXmlScriptFile: description of the xml script file to load.
 * @param script: the script loaded.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool XmlScriptReader::loadScript(QFile *pXmlScriptFile, Script & script) {
#ifdef TRACE_XMLSCRIPTREADER
	{
		std::stringstream log;
		log << "XmlScriptReader::loadScript(";
		log << "xmlScriptFile: " << pXmlScriptFile;
		log << ", script: " << &script;
		log << ")";
		LOG_INFO(log.str());
	}
#endif

	QFile xsdFile(":/XML_SCRIPT_SCHEMA");
	if(!XmlValidator::getInstance().validate(pXmlScriptFile, &xsdFile)) {
		{
			std::stringstream err;
			err << "Xml script is not valid according to Xml schema. Can't load this xml script!";
			LOG_ERROR(err.str());
		}
		return false;
	}
	{
		std::stringstream log;
		log << "The xml document is valid.";
		LOG_DEBUG(log.str());
	}

    QXmlInputSource source( pXmlScriptFile );

    XmlScriptHandler *pScriptHandler = new XmlScriptHandler(script);

    QXmlSimpleReader reader;
    reader.setContentHandler( pScriptHandler );
    reader.setLexicalHandler( pScriptHandler );

    return reader.parse( source );
}

#ifndef _WIN_32
log4cxx::LoggerPtr XmlScriptReader::logger = log4cxx::Logger::getLogger( "controller.xmlParser.XmlScriptReader" );
#endif
