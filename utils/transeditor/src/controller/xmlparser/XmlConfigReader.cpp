/*
 * XmlConfigReader.cpp
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#include "XmlConfigReader.h"

#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#include "XmlConfigHandler.h"
#include "XmlValidator.h"

#define DEBUG_XMLCONFIGREADER
#undef DEBUG_XMLCONFIGREADER

#define TRACE_XMLCONFIGREADER
#undef TRACE_XMLCONFIGREADER

XmlConfigReader *XmlConfigReader::_pInstance = NULL;

/**
 * @brief Get an instance of this XmlScriptReader.
 */
XmlConfigReader* XmlConfigReader::getInstance() {
#ifdef DEBUG_XMLCONFIGREADER
	{
		std::stringstream log;
		log << "XmlConfigReader::getInstance()";
		LOG_DEBUG(log.str());
	}
#endif
	if(!_pInstance) {
		_pInstance = new XmlConfigReader;
	}
	return _pInstance;

}

XmlConfigReader::XmlConfigReader() : QXmlStreamReader() {
#ifdef DEBUG_XMLCONFIGREADER
	{
		std::stringstream log;
		log << "Constructing a new XmlConfigReader object...";
		LOG_DEBUG(log.str());
	}
#endif

}

XmlConfigReader::~XmlConfigReader() {
#ifdef DEBUG_XMLCONFIGREADER
	{
		std::stringstream log;
		log << "Destroying this XmlConfigReader...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Loads script configuration from an xml script.
 *
 * @param pXmlScriptFile: description of the xml config file to load.
 * @param config: the config loaded.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool XmlConfigReader::loadConfig(QFile *pXmlScriptFile, ScriptConfig & config) {
#ifdef TRACE_XMLCONFIGREADER
	{
		std::stringstream log;
		log << "XmlConfigReader::loadConfig(";
		log << "xmlScriptFile: " << pXmlScriptFile->fileName().toStdString();
		log << ", config: " << &config;
		LOG_INFO(log.str());
	}
#endif

	QFile xsdFile(":/XML_CONFIG_SCHEMA");
	if(!XmlValidator::getInstance().validate(pXmlScriptFile, &xsdFile)) {
		{
			std::stringstream err;
			err << "loadConfig() failed - xml script is not valid according to xml schema!";
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

	XmlConfigHandler *pConfigHandler = new XmlConfigHandler(config);

	QXmlSimpleReader reader;
	reader.setContentHandler( pConfigHandler );
	reader.setLexicalHandler( pConfigHandler );

	return reader.parse( source );
}
#ifndef _WIN_32
log4cxx::LoggerPtr XmlConfigReader::logger = log4cxx::Logger::getLogger( "controller.xmlParser.XmlConfigReader" );
#endif
