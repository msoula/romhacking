/*
 * XmlConfigHandler.cpp
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#include "XmlConfigHandler.h"

#define DEBUG_XMLCONFIGHANDLER
#undef DEBUG_XMLCONFIGHANDLER

#define TRACE_XMLCONFIGHANDLER
#undef TRACE_XMLCONFIGHANDLER

#include <QtCore/QRegExp>
#include <QtCore/QString>

#include "XmlScriptTools.h"

/**
 * @brief Constructor of an instance of XmlConfigHandler.
 *
 * @param config: config to load.
 */
XmlConfigHandler::XmlConfigHandler(ScriptConfig &config) : QXmlDefaultHandler() {
#ifdef DEBUG_XMLCONFIGHANDLER
	{
		std::stringstream log;
		log << "Constructing a new XmlConfigHandler object...";
		LOG_DEBUG(log.str());
	}
#endif
	_pCurrentOpcodeData = NULL;
	_pScriptConfig = &config;
	_isWithinCDATA = false;
	_isWithinOpcodeTag = false;
	_currentText = "";
}

/**
 * @brief Destructor of this instance of XmlConfigHandler.
 */
XmlConfigHandler::~XmlConfigHandler() {
#ifdef DEBUG_XMLCONFIGHANDLER
	{
		std::stringstream log;
		log << "Destroying this XmlConfigHandler...";
		LOG_DEBUG(log.str());
	}
#endif
	_pScriptConfig = NULL;
}

bool XmlConfigHandler::characters(const QString & ch)
{
#ifdef TRACE_XMLCONFIGHANDLER
	{
		std::stringstream log;
		log << "XmlConfigHandler::characters(";
		log << "ch: " << ch.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if (_isWithinOpcodeTag && _isWithinCDATA)
	{
		_currentText.append(ch);
	}
	return true;
}

/**
 * This method is called each time a new Element is open
 */
#ifdef TRACE_XmlConfigHandler
bool XmlConfigHandler::startElement(const QString & namespaceURI,
		const QString & localName, const QString & qName,
		const QXmlAttributes & atts)
#else
bool XmlConfigHandler::startElement(const QString &,
		const QString & localName, const QString &,
		const QXmlAttributes & atts)
#endif
{
#ifdef TRACE_XmlConfigHandler
	{
		std::stringstream log;
		log << "XmlConfigHandler::startElement(";
		log << "namespaceURI: " << namespaceURI.toStdString();
		log << ", localName: " << localName.toStdString();
		log << ", qName: " << qName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	bool ok;

	if (localName == XmlScriptTools::CONFIG_ELEMENT_CONFIG)
	{
		QString attMaximumLineSize = atts.value(XmlScriptTools::CONFIG_ATTRIBUTE_MAX_LINE_SIZE);
		_pScriptConfig->setMaximumLineSize(attMaximumLineSize.toInt(&ok, 10));
	}
	else if (localName == XmlScriptTools::CONFIG_ELEMENT_OPCODES)
	{
	}
	else if (localName == XmlScriptTools::CONFIG_ELEMENT_OPCODE)
	{
		_isWithinOpcodeTag = true;
		_pCurrentOpcodeData = new T_OPCODE_DATA;;

		QString attValue = atts.value(XmlScriptTools::CONFIG_ATTRIBUTE_VALUE);

		_pCurrentOpcodeData->value = attValue.toInt(&ok, 10);
	}
	return true;
}

/**
 * This method is called each time a new Element is closed
 */
#ifdef TRACE_XMLCONFIGHANDLER
bool XmlConfigHandler::endElement(const QString & namespaceURI,
		const QString & localName, const QString & qName)
#else
bool XmlConfigHandler::endElement(const QString &,
		const QString & localName, const QString &)
#endif
{
#ifdef TRACE_XMLCONFIGHANDLER
	{
		std::stringstream log;
		log << "XmlConfigHandler::endElement(";
		log << "namespaceURI: " << namespaceURI.toStdString();
		log << ", localName: " << localName.toStdString();
		log << ", qName: " << qName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if (localName == XmlScriptTools::CONFIG_ELEMENT_OPCODE)
	{
		_pScriptConfig->append(*_pCurrentOpcodeData);
		delete _pCurrentOpcodeData;
		_isWithinOpcodeTag = false;
	}
	return true;
}

bool XmlConfigHandler::startCDATA()
{
#ifdef TRACE_XMLCONFIGHANDLER
	{
		std::stringstream log;
		log << "XmlConfigHandler::startCDATA()";
		LOG_INFO(log.str());
	}
#endif
	if(_isWithinOpcodeTag) {
		_isWithinCDATA = true;
	}
	return true;
}

bool XmlConfigHandler::endCDATA()
{
#ifdef TRACE_XMLCONFIGHANDLER
	{
		std::stringstream log;
		log << "XmlConfigHandler::endCDATA()";
		LOG_INFO(log.str());
	}
#endif
	if (_isWithinCDATA)
	{
		_isWithinCDATA = false;
		_pCurrentOpcodeData->regexp = QRegExp(_currentText);
		_currentText.clear();
	}
	return true;
}

#ifndef _WIN_32
log4cxx::LoggerPtr XmlConfigHandler::logger = log4cxx::Logger::getLogger( "controller.xmlParser.XmlConfigHandler" );
#endif
