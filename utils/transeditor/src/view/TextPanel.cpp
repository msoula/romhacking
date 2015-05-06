/*
 * TextPanel.cpp
 *
 *  Created on: 25 oct. 2010
 *      Author: rid
 */

#include "TextPanel.h"

#define DEBUG_TEXTPANEL	// Enable debug mode
#undef DEBUG_TEXTPANEL	// Disable debug mode

#define TRACE_TEXTPANEL	// Enable trace mode
#undef TRACE_TEXTPANEL	// Disable trace mode

#include <QtGui/QSizePolicy>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

#include "../model/ScriptConfig.h"
#include "ScriptHighlighter.h"

TextPanel::TextPanel(const QString & title, QWidget *pParent) : QGroupBox(pParent){
#ifdef DEBUG_TEXTPANEL
	{
		std::stringstream log;
		log << "Constructing a new TextPanel object..." << std::endl;
		log << "\t\ttitle: " << title.toStdString();
		log << ", parent: " << pParent;
		LOG_DEBUG(log.str());
	}
#endif
	_pScriptEditor = NULL;
	_pHighlighter = NULL;
	_isPointedTextLoaded = false;

	initGui(title);
}

TextPanel::~TextPanel() {
#ifdef DEBUG_TEXTPANEL
	{
		std::stringstream log;
		log << "Destroying this TextPanel...";
		LOG_DEBUG(log.str());
	}
#endif
	delete _pHighlighter;
}

/**
 * @brief Clear the content of this TextPanel.
 */
void TextPanel::unloadText() {
#ifdef TRACE_TEXTPANEL
	{
		std::stringstream log;
		log << "TextPanel::unloadText(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pScriptEditor->clear();
	_isPointedTextLoaded = false;
}

/**
 * @brief Load a text.
 *
 * @param text: the text to load.
 * @return TRUE if the operation succeed, FALSE otherwise.
 */
bool TextPanel::loadText(const QString & text) {
#ifdef TRACE_TEXTPANEL
	{
		std::stringstream log;
		log << "TextPanel::loadText(";
		log << "text: '" << text.toStdString() << "'";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pScriptEditor->setText(text);
	_isPointedTextLoaded = true;
	return true;
}

/**
 * @brief Initialize this text panel highlighter.
 *
 * @param config: the config.
 */
void TextPanel::initHighlighter(ScriptConfig *config) {
#ifdef TRACE_TEXTPANEL
	{
		std::stringstream log;
		log << "TextPanel::initHighlighter(";
		log << "config: " << config;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pHighlighter->setHighlightRules(config);
}

//==============================================================================
// GUI METHODS
//==============================================================================
/**
 * @brief Initialize this TextPanel.
 *
 * @param title: the title of this TextPanel.
 */
void TextPanel::initGui(const QString & title) {
#ifdef TRACE_TEXTPANEL
	{
		std::stringstream log;
		log << "TextPanel::initGui()";
		LOG_INFO(log.str());
	}
#endif
	setTitle(title);
	initEditor();

	_pHighlighter = new ScriptHighlighter(_pScriptEditor);

	QVBoxLayout* pLayout = new QVBoxLayout(this);
	pLayout->addWidget(_pScriptEditor);
	setLayout(pLayout);
}

/**
 * @brief Initialize the editor of this TextPanel.
 */
void TextPanel::initEditor() {
#ifdef TRACE_TEXTPANEL
	{
		std::stringstream log;
		log << "TextPanel::initEditor()";
		LOG_INFO(log.str());
	}
#endif
	_pScriptEditor = new QTextEdit(this);
	_pScriptEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	_pScriptEditor->setReadOnly(true);
}

#ifndef _WIN_32
log4cxx::LoggerPtr TextPanel::logger = log4cxx::Logger::getLogger("view.TextPanel");
#endif
