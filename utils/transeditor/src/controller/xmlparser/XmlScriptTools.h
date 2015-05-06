/*
 * XmlScriptTools.h
 *
 *  Created on: 21 févr. 2011
 *      Author: msoula
 */

#ifndef XMLSCRIPTTOOLS_H_
#define XMLSCRIPTTOOLS_H_

#include <QtCore/QString>

class XmlScriptTools {
public:
	const static QString SCRIPT_ELEMENT_DUMP;
	const static QString SCRIPT_ELEMENT_POINTER;

	const static QString SCRIPT_ATTRIBUTE_CPU_ADDRESS;
	const static QString SCRIPT_ATTRIBUTE_TRANSLATED;
	const static QString SCRIPT_ATTRIBUTE_DONE;

	const static QString CONFIG_ELEMENT_CONFIG;
	const static QString CONFIG_ELEMENT_OPCODES;
	const static QString CONFIG_ELEMENT_OPCODE;

	const static QString CONFIG_ATTRIBUTE_MAX_LINE_SIZE;
	const static QString CONFIG_ATTRIBUTE_REGEXP;
	const static QString CONFIG_ATTRIBUTE_VALUE;
};

#endif /* XMLSCRIPTTOOLS_H_ */
