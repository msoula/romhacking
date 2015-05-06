/*
 * TranslationTextPanel.cpp
 *
 *  Created on: 14 nov. 2010
 *      Author: rid
 */

#include "TranslationTextPanel.h"

#define DEBUG_TRANSLATIONTEXTPANEL	// Enable debug mode
#undef DEBUG_TRANSLATIONTEXTPANEL	// Disable debug mode

#define TRACE_TRANSLATIONTEXTPANEL	// Enable trace mode
#undef TRACE_TRANSLATIONTEXTPANEL	// Disable trace mode

#include <QtCore/QEvent>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

/**
 * @brief Constructor of an instance of TranslationTextPanel.
 *
 * @param title: the title of this TextPanel.
 * @param pParent: the parent widget of this TextPanel.
 */
TranslationTextPanel::TranslationTextPanel(const QString & title, QWidget *pParent) : TextPanel(title, pParent) {
#ifdef DEBUG_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "Constructing a new TranslationTextPanel object..." << std::endl;
		log << "\t\ttitle: " << title;
		log << ", parent: " << pParent;
		LOG_DEBUG(log.str());
	}
#endif
	_pResetButton = NULL;
	_pTranslatedButton = NULL;
	_pTranslationDoneButton = NULL;

	initGui();

	scriptEditor()->installEventFilter(this);
	connect(_pTranslatedButton, SIGNAL(toggled(bool)), this, SIGNAL(notifyIsTranslatedButtonToggled(bool)));
	connect(_pTranslationDoneButton, SIGNAL(toggled(bool)), this, SIGNAL(notifyIsTranslationDoneButtonToggled(bool)));
}

TranslationTextPanel::~TranslationTextPanel() {
#ifdef DEBUG_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "Destroying this TranslationTextPanel...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Clear the content of this TextPanel.
 */
void TranslationTextPanel::unloadText() {
#ifdef TRACE_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "TranslationTextPanel::unloadText(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	TextPanel::unloadText();

	scriptEditor()->setReadOnly(true);
	_pTranslatedButton->setChecked(false);
	_pTranslationDoneButton->setChecked(false);
	_pTranslatedButton->setEnabled(false);
	_pTranslationDoneButton->setEnabled(false);
	_pResetButton->setEnabled(false);
}

/**
 * @brief Load a text.
 *
 * @param text: the text to load.
 * @param isTranslated: TRUE if text is translated, FALSE otherwise.
 * @param isTranslationDone: TRUE if text translation is done, FALSE otherwise.
 * @return TRUE if the operation succeed, FALSE otherwise.
 */
bool TranslationTextPanel::loadText(const QString & text, bool isTranslated, bool isTranslationDone) {
#ifdef TRACE_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "TranslationTextPanel::loadText(";
		log << "text: " << &text;
		log << ", isTranslated" << (isTranslated ? "TRUE" : "FALSE");
		log << ", isTranslationDone" << (isTranslationDone ? "TRUE" : "FALSE");
		log << ")";
		LOG_INFO(log.str());
	}
#endif

	TextPanel::loadText(text);

	scriptEditor()->setReadOnly(false);
	_pTranslatedButton->setEnabled(true);
	_pTranslationDoneButton->setEnabled(true);
	_pResetButton->setEnabled(true);

	_pTranslatedButton->setChecked(isTranslated);
	_pTranslationDoneButton->setChecked(isTranslationDone);
	return true;
}
/**
 * @brief Initialize this TranslationTextPanel.
 */
void TranslationTextPanel::initGui() {
#ifdef TRACE_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "TranslationTextPanel::initGui()";
		LOG_INFO(log.str());
	}
#endif
	initButtons();

	QHBoxLayout* pLayout = new QHBoxLayout;
	pLayout->addWidget(_pResetButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pTranslatedButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pTranslationDoneButton, 0, Qt::AlignLeft);
	pLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	QVBoxLayout* pMainLayout = qobject_cast<QVBoxLayout *>(layout());
	if(!pMainLayout) {
		{
			std::stringstream err;
			err << "An error occured while casting text panel layout!";
			LOG_ERROR(err.str());
		}
		return;
	}
	pMainLayout->insertLayout(0, pLayout, 0);

	scriptEditor()->setFont(QFont ("Courier", 11));
}

/**
 * @brief Initialize buttons of this TranslationTextPanel.
 */
void TranslationTextPanel::initButtons() {
#ifdef TRACE_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "TranslationTextPanel::initButtons()";
		LOG_INFO(log.str());
	}
#endif
	_pResetButton = new QPushButton(trUtf8("Reset pointer"), this);
	_pResetButton->setEnabled(false);
	_pTranslatedButton = new QCheckBox(trUtf8("Translated"), this);
	_pTranslatedButton->setEnabled(false);
	_pTranslationDoneButton = new QCheckBox(trUtf8("Sure"), this);
	_pTranslationDoneButton->setEnabled(false);
}

bool TranslationTextPanel::eventFilter ( QObject * watched, QEvent * event ) {
#ifdef TRACE_TRANSLATIONTEXTPANEL
	{
		std::stringstream log;
		log << "TranslationTextPanel::eventFilter(";
		log << "watched: " << watched->metaObject()->className();
		log << ", event: " << event;
		log << ")";
		LOG_INFO(log.str());
	}
#endif

	if(event->type() == QEvent::FocusOut) {
		if(watched == scriptEditor() && isPointedTextLoaded()) {
			Q_EMIT(notifyTranslationTextChg(scriptEditor()->document()->toPlainText()));
		}
	}
	return false;
}

#ifndef _WIN_32
log4cxx::LoggerPtr TranslationTextPanel::logger = log4cxx::Logger::getLogger( "view.TranslationTextPanel" );
#endif
