/*
 * ScriptHighlighter.h
 *
 *  Created on: 23 févr. 2011
 *      Author: msoula
 */

#ifndef SCRIPTHIGHLIGHTER_H_
#define SCRIPTHIGHLIGHTER_H_

#include "../util/Logger.h"

#include <QtGui/QSyntaxHighlighter>

class ScriptConfig;
class ScriptHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	/**
	 * @brief Constructor of an instance of ScriptHighlighter.
	 *
	 * @param editor: text editor.
	 */
	ScriptHighlighter(QTextEdit *editor = 0);

	/**
	 * @brief Destructor of this instance of ScriptHighlighter.
	 */
	~ScriptHighlighter();

	/**
	 * @brief Set highlighting rules.
	 *
	 * @param config: the config file.
	 */
	void setHighlightRules(ScriptConfig *config);

protected:
	/**
	 * @brief Highlights the given text block.
	 *
	 * @param text: the text to treat.
	 */
	void highlightBlock ( const QString & text );

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif

	ScriptConfig *_pConfig;		/*!< config data containing highlighting rules. */

	/**
	 * @brief Check if the given line is too long according to config data.
	 *
	 * @param line: the line.
	 * @return TRUE if line is too long, FALSE otherwise.
	 */
	bool isLineTooLong(const QString & line);

	/**
	 * @brief Highlight a long line.
	 *
	 * @param line: the line to highlight.
	 */
	void highlightLine(const QString & line);

	/**
	 * @brief Highlight opcodes.
	 *
	 * @param line: the line to highlight.
	 */
	void highlightOpcodes(const QString & line);
};

#endif /* SCRIPTHIGHLIGHTER_H_ */
