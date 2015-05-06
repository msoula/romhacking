/*
 * Manager.h
 *
 *  Created on: 17 nov. 2010
 *      Author: rid
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include <QtCore/QObject>

#include "../util/Logger.h"

class MainWindow;
class Script;
class ScriptConfig;
class Manager : public QObject {
	Q_OBJECT
public:
	/**
	 * @brief Constructor of an instance of Manager.
	 *
	 * @param pMainWindow: the MainWindow to manage.
	 */
	Manager(MainWindow* pMainWindow);

	/**
	 * @brief Destructor of this instance of Manager.
	 */
	~Manager();

public Q_SLOTS:
	/**
	 * @brief This slots is executed to load config.
	 *
	 * @param configFileName: xml config file name.
	 */
	void loadConfig(const QString & configFileName);

	/**
	 * @brief This slots is executed to load original script.
	 *
	 * @param originalScriptFileName: file name of original script to load.
	 */
	void loadOriginalScript(const QString & originalScriptFileName);

	/**
	 * @brief This slots is executed to load translation script.
	 *
	 * @param translationScriptFileName: file name of translation script to load.
	 */
	void loadTranslationScript(const QString & translationScriptFileName);

	/**
	 * @brief This slots is executed to unload both scripts.
	 */
	void unloadScripts();

	/**
	 * @brief Modify the current pointed text.
	 *
	 * @param text: the new text.
	 */
	void modifyTranslationText(const QString & text);

	/**
	 * @brief Modify the translation status of the current pointed text.
	 *
	 * @param isTranslated: TRUE if this pointed text is translated, FALSE otherwise.
	 */
	void modifyTranslated(bool isTranslated);

	/**
	 * @brief Modify the translation status of the current pointed text.
	 *
	 * @param isTranslationDone: TRUE if this pointed text translation is done, FALSE otherwise.
	 */
	void modifyTranslationDone(bool isTranslationDone);

	/**
	 * @brief This slots is executed to save the current translated script.
	 */
	void saveTranslation();

	/**
	 * @brief This slot is triggered to set current pointed text.
	 * @param index	new current index
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool current(int index);

	/**
	 * @brief Request for the next pointed text.
	 */
	bool next();

	/**
	 * @brief Request for the next unsure pointed text.
	 */
	bool nextUnsure();

	/**
	 * @brief Request for the next untranslated pointed text.
	 */
	bool nextUntranslated();

	/**
	 * @brief Request for the previous pointed text.
	 */
	bool previous();

	/**
	 * @brief Request for the previous unsure pointed text.
	 */
	bool previousUnsure();

	/**
	 * @brief Request for the previous untranslated pointed text.
	 */
	bool previousUntranslated();

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif

	MainWindow* _pMainWindow;		/*!< the main window of application. */
	Script* _pOriginalScript;		/*!< the original script. */
	Script* _pTranslationScript;	/*!< the translation script. */
	ScriptConfig* _pConfig;			/*!< the script config. */
	int _currentPointedTextIndex;	/*!< the current index. */


	/**
	 * @brief Check given pointed text is available.
	 *
	 * @param index	requested index.
	 * @return TRUE if pointed text at given index is available, FALSE otherwise.
	 */
	bool isAvailable(int index);

	/**
	 * @brief Change the current pointed text.
	 *
	 * @param index: new pointed text index.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool changePointedText(int index);
};

#endif /* MANAGER_H_ */
