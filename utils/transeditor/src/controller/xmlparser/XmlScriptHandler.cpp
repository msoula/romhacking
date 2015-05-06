/*
 * XmlScriptHandler.cpp
 *
 *  Created on: 13 nov. 2010
 *      Author: rid
 */

#define DEBUG_XMLSCRIPTHANDLER	// Enable debug mode
#undef DEBUG_XMLSCRIPTHANDLER	// Disable debug mode

#define TRACE_XMLSCRIPTHANDLER	// Enable trace mode
#undef TRACE_XMLSCRIPTHANDLER	// Disable trace mode

#include "XmlScriptHandler.h"

#include "XmlScriptTools.h"
#include "../../model/PointedText.h"
#include "../../model/Script.h"

XmlScriptHandler::XmlScriptHandler(Script & script) {
#ifdef DEBUG_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "Constructing a new XmlScriptHandler object..." << std::endl;
		log << "\t\tscript: " << &script;
		LOG_DEBUG(log.str());
	}
#endif
	_isWithinCDATA = false;
	_isWithinTextTag = false;
	_pScript = &script;
	_pCurrentPointedText = NULL;
}

XmlScriptHandler::~XmlScriptHandler() {
#ifdef DEBUG_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "Destroying this XmlScriptHandler...";
		LOG_DEBUG(log.str());
	}
#endif
	_pScript = NULL;
	_pCurrentPointedText = NULL;
}

bool XmlScriptHandler::characters(const QString & ch)
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::characters(";
		log << "ch: " << ch.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if (_isWithinTextTag && _isWithinCDATA)
	{
		_currentText.append(ch);
	}
	return true;
}

/**
 * This method is called each time a new Element is open
 */
#ifdef TRACE_XMLSCRIPTHANDLER
bool XmlScriptHandler::startElement(const QString & namespaceURI,
		const QString & localName, const QString & qName,
		const QXmlAttributes & atts)
#else
bool XmlScriptHandler::startElement(const QString &,
		const QString & localName, const QString &,
		const QXmlAttributes & atts)
#endif
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::startElement(";
		log << "namespaceURI: " << namespaceURI.toStdString();
		log << ", localName: " << localName.toStdString();
		log << ", qName: " << qName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if (localName == XmlScriptTools::SCRIPT_ELEMENT_POINTER)
	{
		_pCurrentPointedText = new PointedText;

		QString attCpuAddress = atts.value(XmlScriptTools::SCRIPT_ATTRIBUTE_CPU_ADDRESS);
		_pCurrentPointedText->setCpuAddress(attCpuAddress);

		QString attTranslated = atts.value(XmlScriptTools::SCRIPT_ATTRIBUTE_TRANSLATED);
		_pCurrentPointedText->setTranslated(attTranslated.compare("true", Qt::CaseInsensitive) == 0);

		QString attSure = atts.value(XmlScriptTools::SCRIPT_ATTRIBUTE_DONE);
		_pCurrentPointedText->setTranslationDone(attSure.compare("true", Qt::CaseInsensitive) == 0);
		_isWithinTextTag = true;
		_currentText.clear();
	}
	else if (localName == XmlScriptTools::SCRIPT_ELEMENT_DUMP)
	{
	}
	return true;
}

/**
 * This method is called each time a new Element is closed
 */
#ifdef TRACE_XMLSCRIPTHANDLER
bool XmlScriptHandler::endElement(const QString & namespaceURI,
		const QString & localName, const QString & qName)
#else
bool XmlScriptHandler::endElement(const QString &,
		const QString & localName, const QString &)
#endif
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::endElement(";
		log << "namespaceURI: " << namespaceURI.toStdString();
		log << ", localName: " << localName.toStdString();
		log << ", qName: " << qName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if (localName == XmlScriptTools::SCRIPT_ELEMENT_POINTER)
	{
		_pCurrentPointedText->setText(_currentText);
		_pScript->append(*_pCurrentPointedText);
		_isWithinTextTag = false;
	}
	return true;
}

bool XmlScriptHandler::startCDATA()
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::startCDATA()";
		LOG_INFO(log.str());
	}
#endif
	if (_isWithinTextTag)
	{
		_isWithinCDATA = true;
	}
	return true;
}

bool XmlScriptHandler::endCDATA()
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::endCDATA()";
		LOG_INFO(log.str());
	}
#endif
	if (_isWithinCDATA)
	{
		_isWithinCDATA = false;
	}
	return true;
}

bool XmlScriptHandler::startDocument()
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::startDocument()";
		LOG_INFO(log.str());
	}
#endif
	return QXmlDefaultHandler::startDocument();
}

bool XmlScriptHandler::endDocument ()
{
#ifdef TRACE_XMLSCRIPTHANDLER
	{
		std::stringstream log;
		log << "XmlScriptHandler::endDocument()";
		LOG_INFO(log.str());
	}
#endif
	return QXmlDefaultHandler::endDocument();
}

#ifndef _WIN_32
log4cxx::LoggerPtr XmlScriptHandler::logger = log4cxx::Logger::getLogger( "controller.xmlParser.XmlScriptHandler" );
#endif
