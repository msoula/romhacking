/*
 * XmlConfigHandler.h
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#ifndef XMLCONFIGHANDLER_H_
#define XMLCONFIGHANDLER_H_

#include <QtXml/QXmlDefaultHandler>

#include "../../model/ScriptConfig.h"
#include "../../util/Logger.h"

class XmlConfigHandler : public QXmlDefaultHandler {
public:
	/**
	 * @brief Constructor of an instance of XmlConfigHandler.
	 *
	 * @param config: config to load.
	 */
	XmlConfigHandler(ScriptConfig &config);

	/**
	 * @brief Destructor of this instance of XmlConfigHandler.
	 */
	~XmlConfigHandler();

	//--------------------------------------------------------------------------
	// Inherited methods from QXmlDefaultHandler
	//--------------------------------------------------------------------------
    bool startElement (const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
    bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
    bool startCDATA ();
    bool endCDATA ();
    bool characters ( const QString & ch );

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif

	ScriptConfig *_pScriptConfig;		/*!< script config to load. */
	T_OPCODE_DATA *_pCurrentOpcodeData;	/*!< current opcode data. */
	QString	_currentText;				/*!< current text. */
	bool _isWithinCDATA;				/*!< TRUE if parser is treating CDATA, FALSE otherwise. */
	bool _isWithinOpcodeTag;			/*!< TRUE if parser is treating opcode tag, FALSE otherwise. */
};

#endif /* XMLCONFIGHANDLER_H_ */
