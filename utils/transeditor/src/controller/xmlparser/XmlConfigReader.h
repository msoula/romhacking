/*
 * XmlConfigReader.h
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#ifndef XMLCONFIGREADER_H_
#define XMLCONFIGREADER_H_

#include <QtXml/QXmlStreamReader>

#include "../../model/ScriptConfig.h"
#include "../../util/Logger.h"

class QFile;
class XmlConfigReader : public QXmlStreamReader {
public:

	/**
	 * @brief Get an instance of this XmlScriptReader.
	 */
	static XmlConfigReader* getInstance();

	/**
	 * @brief Loads script configuration from an xml script.
	 *
	 * @param pXmlScriptFile: description of the xml config file to load.
	 * @param config: the config loaded.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool loadConfig(QFile *pXmlScriptFile, ScriptConfig & config);

	/**
	 * @brief Destructor of this instance of XmlConfigReader.
	 */
	~XmlConfigReader();

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif
	static XmlConfigReader *_pInstance;	/*!< unique instance. */

	/**
	 * @brief Constructor of an instance of XmlConfigReader.
	 */
	XmlConfigReader();
};

#endif /* XMLCONFIGREADER_H_ */
