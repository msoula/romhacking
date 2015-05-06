/*
 * XmlValidator.cpp
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#include "XmlValidator.h"

#define DEBUG_XMLVALIDATOR
#undef DEBUG_XMLVALIDATOR

#define TRACE_XMLVALIDATOR
#undef TRACE_XMLVALIDATOR

#include <QtCore/QFile>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

XmlValidator *XmlValidator::_pInstance = NULL;

/**
 * @brief Get an unique instance of XmlValidator.
 *
 * @return the XmlValidator.
 */
XmlValidator &XmlValidator::getInstance() {
#ifdef DEBUG_XMLVALIDATOR
	{
		std::stringstream log;
		log << "XmlValidator::getInstance()";
		LOG_DEBUG(log.str());
	}
#endif
	if(!_pInstance) {
		_pInstance = new XmlValidator;
	}
	return *_pInstance;
}
XmlValidator::XmlValidator() {
#ifdef DEBUG_XMLVALIDATOR
	{
		std::stringstream log;
		log << "Constructing a new XmlValidator object...";
		LOG_DEBUG(log.str());
	}
#endif

}

XmlValidator::~XmlValidator() {
#ifdef DEBUG_XMLVALIDATOR
	{
		std::stringstream log;
		log << "Destroying this XmlValidator...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Validate a XML file according to a given xml schema.
 *
 * @param xmlFile: file to validate.
 * @param xsdFile: xml schema.
 * @return TRUE if xmlFile is valid, FALSE otherwise.
 */
bool XmlValidator::validate(QFile *pXmlFile, QFile *pXsdFile) {
#ifdef TRACE_XMLVALIDATOR
	{
		std::stringstream log;
		log << "XmlValidator::validate(";
		log << "xmlFile: " << pXmlFile->fileName().toStdString();
		log << ", xsdFile: " << pXsdFile->fileName().toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	bool isValid = true;

	if(!pXsdFile->open(QIODevice::ReadOnly)) {
		{
			std::stringstream log;
			log << "validate() failed - xml schema '" << pXsdFile->fileName().toStdString() << "' can't be opened!";
			LOG_ERROR(log.str());
		}
		pXsdFile->close();
		return false;
	}

	QXmlSchema schema;
	schema.load(pXsdFile, QUrl::fromLocalFile(pXsdFile->fileName()));

	if (!schema.isValid()) {
		{
			std::stringstream err;
			err << "validate() failed - xml schema '" << pXsdFile->fileName().toStdString() << "' is not valid!";
			LOG_ERROR(err.str());
		}

		pXsdFile->close();
		return false;
	}

	if(!pXmlFile->isOpen() && !pXmlFile->open(QIODevice::ReadOnly)) {
		{
			std::stringstream err;
			err << "validate() failed - xml file '" << pXsdFile->fileName().toStdString() << "' can't be opened!";
			LOG_ERROR(err.str());
		}

		pXmlFile->close();
		pXsdFile->close();
		return false;
	}

	QXmlSchemaValidator validator(schema);
	isValid = (validator.validate(pXmlFile, QUrl::fromLocalFile(pXmlFile->fileName())));

	pXmlFile->close();
	pXsdFile->close();

	return isValid;
}
#ifndef _WIN_32
log4cxx::LoggerPtr XmlValidator::logger = log4cxx::Logger::getLogger( "controller.xmlParser.XmlValidator" );
#endif
