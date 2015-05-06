/*
 * ScriptHighlighter.cpp
 *
 *  Created on: 23 févr. 2011
 *      Author: msoula
 */

#include "ScriptHighlighter.h"

#define DEBUG_SCRIPTHIGHLIGHTER
#undef DEBUG_SCRIPTHIGHLIGHTER

#define TRACE_SCRIPTHIGHLIGHTER
#undef TRACE_SCRIPTHIGHLIGHTER

#include "../model/ScriptConfig.h"

/**
 * @brief Constructor of an instance of ScriptHighlighter.
 *
 * @param text: text to highlight.
 */
ScriptHighlighter::ScriptHighlighter(QTextEdit *pEditor) : QSyntaxHighlighter(pEditor) {
#ifdef DEBUG_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "Constructing a new ScriptHighlighter object...";
		log << "\t\teditor: " << pEditor;
		LOG_DEBUG(log.str());
	}
#endif
	_pConfig = NULL;
}

/**
 * @brief Destructor of this instance of ScriptHighlighter.
 */
ScriptHighlighter::~ScriptHighlighter() {
#ifdef DEBUG_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "Destroying this ScriptHighlighter...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Set highlighting rules.
 *
 * @param config: the config file.
 */
void ScriptHighlighter::setHighlightRules(ScriptConfig *pConfig) {
#ifdef TRACE_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "ScriptHighlighter::setHighlightRules(";
		log << "config: " << pConfig;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(_pConfig == pConfig) {
		return;
	}

	_pConfig = pConfig;
	rehighlight();
}

/**
 * @brief Highlights the given text block.
 *
 * @param text: the text to treat.
 */
void ScriptHighlighter::highlightBlock(const QString & text) {
#ifdef TRACE_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "ScriptHighlighter::highlightBlock(";
		log << "text: " << text.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif

	if(!_pConfig) {
#ifdef DEBUG_SCRIPTHIGHLIGHTER
		{
			std::stringstream log;
			log << "highlighBlock() - no config has been set: no highlighting!";
			LOG_DEBUG(log.str());
		}
#endif
		return;
	}

	highlightOpcodes(text);
	if(isLineTooLong(text)) {
		highlightLine(text);
	}
}

/**
 * @brief Check if the given line is too long according to config data.
 *
 * @param line: the line.
 * @return TRUE if line is too long, FALSE otherwise.
 */
bool ScriptHighlighter::isLineTooLong(const QString & line) {
#ifdef TRACE_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "ScriptHighlighter::isLineTooLong(";
		log << "line: " << line.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	int byteCounter = 0;

	QString lineWithoutOpCode(line);
	int opcodeByteCounter;
	QListIterator<T_OPCODE_DATA> i(*(_pConfig->opcodes()));
	while (i.hasNext())
	{
		const T_OPCODE_DATA *pOpcodeData = &(i.next());
		opcodeByteCounter = line.count(pOpcodeData->regexp) * pOpcodeData->value;
		if (opcodeByteCounter >= 0)
		{
			byteCounter += opcodeByteCounter;
			lineWithoutOpCode = lineWithoutOpCode.split(pOpcodeData->regexp).join("");
		}
	}

#ifdef DEBUG_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "line without opcode: " << lineWithoutOpCode.toStdString() << std::endl;
		log << "opcode byte length: " << byteCounter;
		log << "line without opcode length: " << lineWithoutOpCode.size();
		LOG_DEBUG(log.str());
	}
#endif

	byteCounter += lineWithoutOpCode.size();

	return byteCounter > _pConfig->maximumLineSize();
}

/**
 * @brief Highlight a long line.
 *
 * @param line: the line to highlight.
 */
void ScriptHighlighter::highlightLine(const QString & line) {
#ifdef TRACE_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "ScriptHighlighter::highlightLine(";
		log << "line: " << line.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	QFont longLineFont;
	QTextCharFormat format;
	format.setBackground(QColor(0xFF, 0x73, 0x73));
	setFormat(0, line.size(), format);
}

/**
 * @brief Highlight opcodes.
 *
 * @param line: the line to highlight.
 */
void ScriptHighlighter::highlightOpcodes(const QString & line) {
#ifdef TRACE_SCRIPTHIGHLIGHTER
	{
		std::stringstream log;
		log << "ScriptHighlighter::highlightOpcodes(";
		log << "line: " << line.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	QTextCharFormat format;
	format.setForeground(Qt::darkMagenta);
	format.setFontWeight(QFont::Bold);

	Q_FOREACH(const T_OPCODE_DATA & opcode, *(_pConfig->opcodes()))
	{
		int index = opcode.regexp.indexIn(line);
		while (index >= 0)
		{
			int length = opcode.regexp.matchedLength();
			setFormat(index, length, format);
			index = opcode.regexp.indexIn(line, index + length);
		}
	}
}

#ifndef _WIN_32
log4cxx::LoggerPtr ScriptHighlighter::logger = log4cxx::Logger::getLogger( "view.ScriptHighlighter" );
#endif
