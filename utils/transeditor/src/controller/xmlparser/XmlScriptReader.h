/*
 * XmlScriptReader.h
 *
 *  Created on: 1 nov. 2010
 *      Author: rid
 */

#ifndef XMLSCRIPTREADER_H_
#define XMLSCRIPTREADER_H_

#include <QtCore/QObject>

#include "../../model/Script.h"
#include "../../util/Logger.h"

class QFile;

class XmlScriptReader : public QObject {
public:
	/**
	 * @brief Destructor of this instance of XmlScriptReader.
	 */
	~XmlScriptReader();

	/**
	 * @brief Get an instance of this XmlScriptReader.
	 */
	static XmlScriptReader* getInstance();

	/**
	 * @brief Loads a Script from an xml script.
	 *
	 * @param pXmlScriptFile: description of the xml script file to load.
	 * @param script: the script loaded.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool loadScript(QFile *pXmlScriptFile, Script & script);

private:
	static XmlScriptReader* _pInstance;	/*!< unique instance of XmlScriptReader. */

	/**
	 * @brief Constructor of an instance of XmlScriptReader.
	 */
	XmlScriptReader();

#ifndef _WIN_32
	static log4cxx::LoggerPtr logger; /*!< the logger. */
#endif
};

#endif /* XMLSCRIPTREADER_H_ */
