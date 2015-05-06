/*
 * XmlScriptHandler.h
 *
 *  Created on: 13 nov. 2010
 *      Author: rid
 */

#ifndef XMLSCRIPTHANDLER_H_
#define XMLSCRIPTHANDLER_H_

#include <QtXml/QXmlDefaultHandler>

#include "../../util/Logger.h"

class PointedText;
class Script;

class XmlScriptHandler: public QXmlDefaultHandler {
public:

	/**
	 * @brief Constructor of an instance of XmlScriptHandler.
	 *
	 * @param script: the script to load.
	 */
	XmlScriptHandler(Script & script);

	/**
	 * @brief Destructor of this instance of XmlScriptHandler.
	 */
	~XmlScriptHandler();

	//--------------------------------------------------------------------------
	// Inherited methods from QXmlDefaultHandler
	//--------------------------------------------------------------------------
    bool startDocument ();
    bool endDocument ();
    bool startElement (const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
    bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
    bool startCDATA ();
    bool endCDATA ();
    bool characters ( const QString & ch );

private:
    Script * _pScript;					/*!< the script loaded from xml file. */
    PointedText * _pCurrentPointedText; /*!< the PointedText currently treated. */
    QString _currentText;				/*!< the text currently read. */
    bool _isWithinTextTag;				/*!< TRUE if parsing text tag, FALSE otherwise. */
    bool _isWithinCDATA;				/*!< TRUE if parsing text CDATA, FALSE otherwise. */

#ifndef _WIN_32
	static log4cxx::LoggerPtr logger; /*!< the logger. */
#endif
};

#endif /* XMLSCRIPTHANDLER_H_ */
