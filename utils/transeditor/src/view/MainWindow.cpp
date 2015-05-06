/*
 * MainWindow.cpp
 *
 *  Created on: 14 nov. 2010
 *      Author: rid
 */

#include "MainWindow.h"

#define DEBUG_MAINWINDOW	// Enable debug mode
//#undef DEBUG_MAINWINDOW		// Disable debug mode

#define TRACE_MAINWINDOW
#undef TRACE_MAINWINDOW

#include <QtCore/QDir>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <stdio.h>

#include "ControlBar.h"
#include "TextPanel.h"
#include "TranslationTextPanel.h"
#include "../model/Script.h"
#include "../model/ScriptConfig.h"

const QString MainWindow::MENU_FILE_TEXT 			= trUtf8("&File");			/*!< menu file text. */
const QString MainWindow::ACTION_OPEN_TEXT 			= trUtf8("Open...");		/*!< action open text. */
const QString MainWindow::ACTION_SAVE_TEXT 			= trUtf8("Save...");		/*!< action save text. */
const QString MainWindow::ACTION_LOAD_CONFIG_TEXT	= trUtf8("Load Config...");	/*!< action load config text. */
const QString MainWindow::ACTION_CLOSE_TEXT 		= trUtf8("Close");			/*!< action close text. */
const QString MainWindow::ACTION_QUIT_TEXT 			= trUtf8("Quit");			/*!< action quit text. */
const QString MainWindow::MENU_EDIT_TEXT 			= trUtf8("&Edit");			/*!< menu edit text. */
const QString MainWindow::ACTION_SEARCH_TEXT 		= trUtf8("Search...");		/*!< action search text. */
const QString MainWindow::MENU_HELP_TEXT 			= trUtf8("&?");				/*!< menu help text. */
const QString MainWindow::ACTION_ABOUT_TEXT 		= trUtf8("About...");		/*!< action about text. */

MainWindow::MainWindow(QWidget * pParent, Qt::WindowFlags flags) : QMainWindow(pParent, flags) {
#ifdef DEBUG_MAINWINDOW
	LOG_CONSTRUCTOR("MainWindow");
#endif
	_pControlBar = NULL;
	_pOriginalPanel = NULL;
	_pTranslationPanel = NULL;

	initGui();
}

MainWindow::~MainWindow() {
#ifdef DEBUG_MAINWINDOW
	LOG_DESTROYER("MainWindow");
#endif
}

/**
 * @brief Unload text panels.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool MainWindow::unloadTextPanels() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::unloadTextPanels(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pOriginalPanel->unloadText();
	_pTranslationPanel->unloadText();
	_actions[E_ACTIONS_ID_LOAD_CONFIG]->setEnabled(false);
	_actions[E_ACTIONS_ID_SAVE]->setEnabled(false);
	_actions[E_ACTIONS_ID_CLOSE]->setEnabled(false);
	return true;
}

/**
 * @brief Set the pointed text.
 *
 * @param index: index of the pointed text.
 * @param originalText: original text.
 * @parant translationText: translation text.
	 * @param isTranslated: TRUE if text is translated, FALSE otherwise.
	 * @param isTranslationDone: TRUE if text translation is done, FALSE otherwise.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool MainWindow::setPointedText(int index, const QString & originalText, const QString & translationText, bool isTranslated, bool isTranslationDone) {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::setPointedText(";
		log << "index: " << index;
		log << ", originalText: " << originalText.toStdString();
		log << ", translationText: " << translationText.toStdString();
		log << ", isTranslated" << (isTranslated ? "TRUE" : "FALSE");
		log << ", isTranslationDone" << (isTranslationDone ? "TRUE" : "FALSE");
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pControlBar->setNumber(index);

	_pOriginalPanel->unloadText();
	_pOriginalPanel->loadText(originalText);

	_pTranslationPanel->unloadText();
	_pTranslationPanel->loadText(translationText, isTranslated, isTranslationDone);

	_actions[E_ACTIONS_ID_LOAD_CONFIG]->setEnabled(true);
	_actions[E_ACTIONS_ID_SAVE]->setEnabled(true);
	_actions[E_ACTIONS_ID_CLOSE]->setEnabled(true);

	return true;
}

/**
 * @brief Set highlight rules to text panels.
 *
 * @param config: script config that contains highlighting rules.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool MainWindow::setHighlighter(ScriptConfig *pConfig) {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::setHighlighter(";
		log << "config: " << pConfig;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pOriginalPanel->initHighlighter(pConfig);
	_pTranslationPanel->initHighlighter(pConfig);
	return true;
}

//==============================================================================
// GUI METHODS
//==============================================================================
/**
 * @brief Initialize this MainWindow.
 */
void MainWindow::initGui() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::initGui()";
		LOG_INFO(log.str());
	}
#endif
	setupMenuBar();
	initControlBar();
	initTextPanels();
	setupStatusBar();

    setCentralWidget(new QWidget(this));
	QGridLayout* pLayout = new QGridLayout;
	pLayout->addWidget(_pControlBar, 0, 0, 1, 2, Qt::AlignTop);
	pLayout->addWidget(_pOriginalPanel, 1, 0, 1, 1);
	pLayout->addWidget(_pTranslationPanel, 1, 1, 1, 1);
	centralWidget()->setLayout(pLayout);
}

/**
 * @brief Initialize the control bar of this MainWindow.
 */
void MainWindow::initControlBar() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::initControlBar()";
		LOG_INFO(log.str());
	}
#endif
	_pControlBar = ControlBar::instance();
	if(!_pControlBar->parent()) {
		_pControlBar->setParent(this);
	}
}

/**
 * @brief Initialize text panels of this MainWindow.
 */
void MainWindow::initTextPanels() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::initTextPanels()";
		LOG_INFO(log.str());
	}
#endif
	_pOriginalPanel = new TextPanel(trUtf8("Original version"), this);
	_pTranslationPanel = new TranslationTextPanel(trUtf8("Translation version"), this);
}

/**
 * @brief Set up the menu bar of this MainWindow.
 */
void MainWindow::setupMenuBar() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::setupMenuBar()";
		LOG_INFO(log.str());
	}
#endif

    QMenu *menu = menuBar()->addMenu(MENU_FILE_TEXT);
    QAction *action = menu->addAction(ACTION_OPEN_TEXT, this, SLOT(open()));
    action->setShortcut(Qt::CTRL + Qt::Key_O);
    _actions[E_ACTIONS_ID_OPEN] = action;
    action = menu->addAction(ACTION_LOAD_CONFIG_TEXT, this, SLOT(loadConfig()));
    _actions[E_ACTIONS_ID_LOAD_CONFIG] = action;
    action = menu->addAction(ACTION_SAVE_TEXT, this, SIGNAL(notifySaveActionClick()));
    action->setShortcut(Qt::CTRL + Qt::Key_S);
    _actions[E_ACTIONS_ID_SAVE] = action;
    action = menu->addAction(ACTION_CLOSE_TEXT, this, SIGNAL(notifyCloseActionClick()));
    action->setShortcut(Qt::CTRL + Qt::Key_W);
    _actions[E_ACTIONS_ID_CLOSE] = action;
    action = menu->addAction(ACTION_QUIT_TEXT, this, SLOT(close()));
    action->setShortcut(Qt::CTRL + Qt::Key_Q);
    _actions[E_ACTIONS_ID_QUIT] = action;

    menu = menuBar()->addMenu(MENU_EDIT_TEXT);
    action = menu->addAction(ACTION_SEARCH_TEXT, this, SIGNAL(notifySearchActionClick()));
    action->setShortcut(Qt::CTRL + Qt::Key_F);
    _actions[E_ACTIONS_ID_SEARCH] = action;

    menu = menuBar()->addMenu(MENU_HELP_TEXT);
    action = menu->addAction(ACTION_ABOUT_TEXT, this, SLOT(about()));
    _actions[E_ACTIONS_ID_ABOUT] = action;

	_actions[E_ACTIONS_ID_LOAD_CONFIG]->setEnabled(false);
	_actions[E_ACTIONS_ID_SAVE]->setEnabled(false);
	_actions[E_ACTIONS_ID_CLOSE]->setEnabled(false);
}

/**
* @brief Set up the status bar of this MainWindow.
*/
void MainWindow::setupStatusBar() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::setupStatusBar()";
		LOG_INFO(log.str());
	}
#endif
    statusBar()->showMessage(trUtf8("Ready"), 5000);
}

//==============================================================================
// SLOTS
//==============================================================================

/**
 * @brief This slots is executed when about frame has to be displayed.
 */
void MainWindow::about() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::about()";
		LOG_INFO(log.str());
	}
#endif
	QMessageBox::about(this, trUtf8("About XmlScriptEditor"),
	             trUtf8("<p>The <b>SPCScriptEditor</b> is an editor used to "
	                "translate easily <i>Samurai Pizza Cats</i> scripts.</p>"
	                "<p>Developped by Mathieu Soula (aka Rid), Terminus "
	                "Traduction</p>"));
}

/**
 * @brief This slots is executed when open menu is clicked.
 */
void MainWindow::open() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::open()";
		LOG_INFO(log.str());
	}
#endif
	QString originalFileName = QFileDialog::getOpenFileName(this,
		                       trUtf8("Open original script"), "."/*QDir::home().absolutePath()*/, trUtf8("XML files (*.xml)"));
	if(originalFileName.isEmpty()) {
		{
			std::stringstream log;
			log << "open() failed - no original script file has been selected!";
			LOG_DEBUG(log.str());
		}
		return;
	}

	QString translationFileName = QFileDialog::getOpenFileName(this,
		                       trUtf8("Open translation script"), originalFileName.left(originalFileName.lastIndexOf('/')), trUtf8("XML files (*.xml)"));
	if(translationFileName.isEmpty()) {
		{
			std::stringstream log;
			log << "open failed() - no translation script file has been selected!";
			LOG_DEBUG(log.str());
		}
		return;
	}

	Q_EMIT(notifyLoadOriginalScript(originalFileName));
	Q_EMIT(notifyLoadTranslationScript(translationFileName));
}


/**
 * @brief This slots is executed when load config menu is clicked.
 */
void MainWindow::loadConfig() {
#ifdef TRACE_MAINWINDOW
	{
		std::stringstream log;
		log << "MainWindow::loadConfig()";
		LOG_INFO(log.str());
	}
#endif
	QString originalFileName = QFileDialog::getOpenFileName(this,
							   trUtf8("Open script config"), "." /*QDir::home().absolutePath()*/, trUtf8("XML files (*.xml)"));
	if(originalFileName.isEmpty()) {
		{
			std::stringstream log;
			log << "loadConfig() failed - no script config file has been selected!";
			LOG_DEBUG(log.str());
		}
		return;
	}

	Q_EMIT(notifyLoadScriptConfig(originalFileName));
}

#ifndef _WIN_32
log4cxx::LoggerPtr MainWindow::logger = log4cxx::Logger::getLogger( "view.MainWindow" );
#endif
