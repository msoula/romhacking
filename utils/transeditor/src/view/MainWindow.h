/*
 * MainWindow.h
 *
 *  Created on: 14 nov. 2010
 *      Author: rid
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui/QMainWindow>

#include "../util/Logger.h"

class ControlBar;
class Script;
class ScriptConfig;
class TextPanel;
class TranslationTextPanel;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	/**
	 * @enum TEXT_PANEL_TYPE
	 * @brief type of text panel.
	 */
	enum TEXT_PANEL_TYPE
	{
		TEXT_PANEL_ORIGINAL,
		TEXT_PANEL_TRANSLATION,
		TEXT_PANEL_MAX,
		TEXT_PANEL_UNKNOWN
	};

	/**
	 * @brief Constructor of an instance of MainWindow.
	 */
	MainWindow(QWidget * pParent = 0, Qt::WindowFlags flags = 0);

	/**
	 * @brief Destructor of this instance of MainWindow.
	 */
	~MainWindow();

	/**
	 * @brief Unload a text in a script text panel.
	 *
	 * @param textId: identifier of the text panel where to load text.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool unloadTextPanels();

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
	bool setPointedText(int index, const QString & originalText, const QString & translationText, bool isTranslated, bool isTranslationDone);

	/**
	 * @brief Set highlight rules to text panels.
	 *
	 * @param config: script config that contains highlighting rules.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool setHighlighter(ScriptConfig *config);

	/**
	 * @brief Get the control bar.
	 *
	 * @return the ControlBar.
	 */
	inline ControlBar *controlBar() {
		return _pControlBar;
	}

	/**
	 * @brief Get the translation text panel.
	 *
	 * @return the translation text panel..
	 */
	inline TranslationTextPanel *translationTextPanel() {
		return _pTranslationPanel;
	}

public Q_SLOTS:
	/**
	 * @brief This slots is executed when about frame has to be displayed.
	 */
	void about();

	/**
	 * @brief This slots is executed when open menu is clicked.
	 */
	void open();

	/**
	 * @brief This slots is executed when load config menu is clicked.
	 */
	void loadConfig();

Q_SIGNALS:
	/**
	 * @brief This signal is triggered when a config has to be loaded.
	 *
	 * @param configFileName: the file name of config.
	 */
	void notifyLoadScriptConfig(const QString & configFileName);

	/**
	 * @brief This signal is triggered when an original script has to be loaded.
	 *
	 * @param originalScriptFileName: the file name of original script to load.
	 */
	void notifyLoadOriginalScript(const QString & originalScriptFileName);

	/**
	 * @brief This signal is triggered when a translation script has to be loaded.
	 *
	 * @param notifyLoadTranslationScript: the file name of translation script to load.
	 */
	void notifyLoadTranslationScript(const QString & translationScriptFileName);

	/**
	 * @brief This signal is emitted when user clicks on close action.
	 */
	void notifyCloseActionClick();

	/**
	 * @brief This signal is triggered when user clicks on save action.
	 */
	void notifySaveActionClick();

	/**
	 * @brief This signal is triggered when user clicks on search action.
	 */
	void notifySearchActionClick();

private:
	/**
	 * @enum E_ACTIONS_ID_TYPE
	 * @brief menu bar actions identifiers.
	 */
	enum E_ACTIONS_ID_TYPE
	{
		E_ACTIONS_ID_OPEN,
		E_ACTIONS_ID_LOAD_CONFIG,
		E_ACTIONS_ID_SAVE,
		E_ACTIONS_ID_CLOSE,
		E_ACTIONS_ID_QUIT,
		E_ACTIONS_ID_SEARCH,
		E_ACTIONS_ID_ABOUT,
		E_ACTIONS_ID_MAX,
		E_ACTIONS_ID_UNKNOWN
	};

#ifndef _WIN_32
	static log4cxx::LoggerPtr logger; /*!< the logger. */
#endif
	static const QString MENU_FILE_TEXT;			/*!< menu file text. */
	static const QString ACTION_OPEN_TEXT;			/*!< action open text. */
	static const QString ACTION_SAVE_TEXT;			/*!< action save text. */
	static const QString ACTION_LOAD_CONFIG_TEXT;	/*!< action load config text. */
	static const QString ACTION_CLOSE_TEXT;			/*!< action close text. */
	static const QString ACTION_QUIT_TEXT;			/*!< action quit text. */
	static const QString MENU_EDIT_TEXT;			/*!< menu edit text. */
	static const QString ACTION_SEARCH_TEXT;		/*!< action search text. */
	static const QString MENU_HELP_TEXT;			/*!< menu help text. */
	static const QString ACTION_ABOUT_TEXT;			/*!< action about text. */

	ControlBar* _pControlBar;						/*!< the bar that controls script navigation. */
	TextPanel* _pOriginalPanel;					/*!< the panel that contains original script. */
	TranslationTextPanel* _pTranslationPanel;	/*!< the panel that contains script to translate. */

	QAction *_actions[E_ACTIONS_ID_MAX];		/*!< list of menu bar actions. */

	/**
	 * @brief Initialize this MainWindow.
	 */
	void initGui();

	/**
	 * @brief Initialize the control bar of this MainWindow.
	 */
	void initControlBar();

	/**
	 * @brief Initialize text panels of this MainWindow.
	 */
	void initTextPanels();

	/**
	 * @brief Set up the menu bar of this MainWindow.
	 */
	void setupMenuBar();

	/**
	* @brief Set up the status bar of this MainWindow.
	*/
	void setupStatusBar();

};

#endif /* MAINWINDOW_H_ */
