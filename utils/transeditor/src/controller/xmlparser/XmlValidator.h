/*
 * XmlValidator.h
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#ifndef XMLVALIDATOR_H_
#define XMLVALIDATOR_H_

#include "../../util/Logger.h"

class QFile;
class XmlValidator {
public:
	/**
	 * @brief Get an unique instance of XmlValidator.
	 *
	 * @return the XmlValidator.
	 */
	static XmlValidator &getInstance();

	/**
	 * @brief Destructor of this instance of XmlValidator.
	 */
	~XmlValidator();

	/**
	 * @brief Validate a XML file according to a given xml schema.
	 *
	 * @param xmlFile: file to validate.
	 * @param xsdFile: xml schema.
	 * @return TRUE if xmlFile is valid, FALSE otherwise.
	 */
	bool validate(QFile *xmlFile, QFile *xsdFile);

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif
	static XmlValidator *_pInstance;	/*!< unique instance. */

	/**
	 * @brief Constructor of an instance of XmlValidator.
	 */
	XmlValidator();
};

#endif /* XMLVALIDATOR_H_ */
