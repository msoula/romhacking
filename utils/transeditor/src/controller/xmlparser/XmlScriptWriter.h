/*
 * XmlScriptWriter.h
 *
 *  Created on: 19 nov. 2010
 *      Author: rid
 */

#ifndef XMLSCRIPTWRITER_H_
#define XMLSCRIPTWRITER_H_

#include "../../util/Logger.h"

#include <QtXml/QXmlStreamWriter>

class PointedText;
class Script;
class XmlScriptWriter : public QXmlStreamWriter {
public:
	/**
	 * @brief Get unique instance of this XmlScriptWriter.
	 *
	 * @return unique instance of this XmlScriptWriter.
	 */
	static XmlScriptWriter *getInstance();

	/**
	 * @brief Write a given script into xml file.
	 *
	 * @param script: script.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool writeScript(Script *script);

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif
	static XmlScriptWriter* _pInstance;	/*!< this instance. */

	/**
	 * @brief Constructor of an instance of XmlScriptWriter.
	 */
	XmlScriptWriter();

	/**
	 * @brief Destructor of this instance of XmlScriptWriter.
	 */
	~XmlScriptWriter();

	/**
	 * @brief Write a given pointed text into xml file.
	 *
	 * @param pointedText: pointed text to write.
	 */
	void writePointedText(PointedText *pointedText);
};

#endif /* XMLSCRIPTWRITER_H_ */
