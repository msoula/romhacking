/*
 * TextPanel.h
 *
 *  Created on: 25 oct. 2010
 *      Author: rid
 */

#ifndef TEXTPANEL_H_
#define TEXTPANEL_H_

#include <QtGui/QGroupBox>

#include "../util/Logger.h"

class PointedText;
class QTextEdit;
class ScriptConfig;
class ScriptHighlighter;
class TextPanel : public QGroupBox {
	Q_OBJECT
public:
	/**
	 * @brief Constructor of an instance of TextPanel.
	 *
	 * @param title: the title of this TextPanel.
	 * @param parent: the parent widget of this TextPanel.
	 */
	TextPanel(const QString & title, QWidget *pParent);

	/**
	 * @brief Destructor of this instance of TextPanel.
	 */
	virtual ~TextPanel();

	/**
	 * @brief Clear the content of this TextPanel.
	 */
	void unloadText();

	/**
	 * @brief Load a text.
	 *
	 * @param text: the text to load.
	 * @return TRUE if the operation succeed, FALSE otherwise.
	 */
	bool loadText(const QString & text);

	/**
	 * @brief Check a pointed text is currently loaded.
	 *
	 * @return TRUE if a pointed text is loaded, FALSE otherwise.
	 */
	inline bool isPointedTextLoaded() const {
		return _isPointedTextLoaded;
	}

	/**
	 * @brief Initialize this text panel highlighter.
	 *
	 * @param config: the config.
	 */
	void initHighlighter(ScriptConfig *config);

protected:
	inline QTextEdit* scriptEditor() {
		return _pScriptEditor;
	}

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif

	ScriptHighlighter *_pHighlighter;	/*!< the highlighter. */
	QTextEdit* _pScriptEditor;	/*!< the editor that contains script. */
	bool _isPointedTextLoaded;	/*!< TRUE if a pointed text is currently loaded, FALSE otherwise. */

	/**
	 * @brief Initialize this TextPanel.
	 *
	 * @param title: the title of this TextPanel.
	 */
	void initGui(const QString & title);

	/**
	 * @brief Initialize the editor of this TextPanel.
	 */
	void initEditor();
};

#endif /* TEXTPANEL_H_ */
