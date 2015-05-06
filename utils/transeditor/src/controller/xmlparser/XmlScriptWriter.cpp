/*
 * XmlScriptWriter.cpp
 *
 *  Created on: 19 nov. 2010
 *      Author: rid
 */

#include "XmlScriptWriter.h"

#define DEBUG_XMLSCRIPTWRITER
#undef DEBUG_XMLSCRIPTWRITER

#define TRACE_XMLSCRIPTWRITER
#undef TRACE_XMLSCRIPTWRITER

#include <QtCore/QFile>

#include "XmlScriptTools.h"
#include "XmlValidator.h"
#include "../../model/PointedText.h"
#include "../../model/Script.h"

XmlScriptWriter *XmlScriptWriter::_pInstance = NULL;

#define NS_URI			("http://www.w3.org/2001/XMLSchema-instance")
#define NS_PREFIX		("xsi")
#define NS_XSE_URI		("http://terminus.romhack.net/XmlScriptEditor_script")
#define NS_XSE_PREFIX	("xse_s")

/**
 * @brief Get unique instance of this XmlScriptWriter.
 *
 * @return unique instance of this XmlScriptWriter.
 */
XmlScriptWriter *XmlScriptWriter::getInstance() {
#ifdef TRACE_XMLSCRIPTWRITER
	{
		std::stringstream log;
		log << "XmlScriptWriter::getInstance()";
		LOG_INFO(log.str());
	}
#endif
	if(!_pInstance) {
		_pInstance = new XmlScriptWriter;
	}
	return _pInstance;
}

/**
 * @brief Constructor of an instance of XmlScriptWriter.
 */
XmlScriptWriter::XmlScriptWriter() : QXmlStreamWriter() {
#ifdef DEBUG_XMLSCRIPTWRITER
	{
		std::stringstream log;
		log << "Constructing a new XmlScriptWriter object...";
		LOG_DEBUG(log.str());
	}
#endif
    setAutoFormatting(true);
}

/**
 * @brief Destructor of this instance of XmlScriptWriter.
 */
XmlScriptWriter::~XmlScriptWriter() {
#ifdef DEBUG_XMLSCRIPTWRITER
	{
		std::stringstream log;
		log << "Destroying this XmlScriptWriter...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Write a given script into xml file.
 *
 * @param file: xml file.
 * @param script: script.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool XmlScriptWriter::writeScript(Script *pScript) {
#ifdef TRACE_XMLSCRIPTWRITER
	{
		std::stringstream log;
		log << "XmlScriptWriter::writeScript(";
		log << "file: " << file;
		log << ", script: " << &pScript;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	QFile *pTmpFile = new QFile(pScript->fileName() + ".sav");
	if (!pTmpFile->open(QIODevice::WriteOnly)) {
		{
			std::stringstream log;
			log << "writeScript() failed - cannot open file '" << pTmpFile->fileName().toStdString() << "'!";
			LOG_ERROR(log.str());
		}
		delete pTmpFile;
		return false;
	}
	setDevice(pTmpFile);

	writeStartDocument();
	writeNamespace(QString::fromUtf8(NS_URI), QString::fromUtf8(NS_PREFIX));
	writeNamespace(QString::fromUtf8(NS_XSE_URI), QString::fromUtf8(NS_XSE_PREFIX));
	writeStartElement(QString::fromUtf8(NS_XSE_URI), XmlScriptTools::SCRIPT_ELEMENT_DUMP);

	for(int i = 0; i < pScript->size(); i++) {
		writePointedText(pScript->getPointedText(i));
	}

    writeEndElement();
    writeEndDocument();
    setDevice(NULL);
    pTmpFile->close();

    QFile xsdFile(":/XML_SCRIPT_SCHEMA");
	if(!XmlValidator::getInstance().validate(pTmpFile, &xsdFile)) {
		{
			std::stringstream err;
			err << "Xml script is not valid according to Xml schema. Can't save this xml script!";
			LOG_ERROR(err.str());
		}
		return false;
	}

	if(QFile::exists(pScript->fileName()))
		QFile::remove(pScript->fileName());

    pTmpFile->rename(pScript->fileName());
    delete pTmpFile;
	return true;
}

//=============================================================================
// PRIVATE METHODS
//=============================================================================

/**
 * @brief Write a given pointed text into xml file.
 *
 * @param pointedText: pointed text to write.
 */
void XmlScriptWriter::writePointedText(PointedText *pPointedText) {
#ifdef TRACE_XMLSCRIPTWRITER
	{
		std::stringstream log;
		log << "XmlScriptWriter::writePointedText(";
		log << "pointedText: " << pPointedText;
		log << ")";
		LOG_INFO(log.str());
	}
#endif

    writeStartElement(QString::fromUtf8(NS_XSE_URI), XmlScriptTools::SCRIPT_ELEMENT_POINTER);
    writeAttribute(XmlScriptTools::SCRIPT_ATTRIBUTE_CPU_ADDRESS, pPointedText->getCpuAddress());
    writeAttribute(XmlScriptTools::SCRIPT_ATTRIBUTE_TRANSLATED, pPointedText->isTranslated() ? "true" : "false");
    writeAttribute(XmlScriptTools::SCRIPT_ATTRIBUTE_DONE, pPointedText->isTranslationDone() ? "true" : "false");

    writeCDATA(pPointedText->text());

    writeEndElement();
};

#ifndef _WIN_32
log4cxx::LoggerPtr XmlScriptWriter::logger = log4cxx::Logger::getLogger( "controller.xmlParser.XmlScriptWriter" );
#endif
