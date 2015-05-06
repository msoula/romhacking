/*
 * Manager.cpp
 *
 *  Created on: 17 nov. 2010
 *      Author: rid
 */

#include "Manager.h"

#define DEBUG_MANAGER	// Enable debug mode
//#undef DEBUG_MANAGER	// Disable debug mode

#define TRACE_MANAGER	// Enable trace
//#undef TRACE_MANAGER	// Disable trace

#include <QtCore/QFile>

#include "xmlparser/XmlConfigReader.h"
#include "xmlparser/XmlScriptReader.h"
#include "xmlparser/XmlScriptWriter.h"
#include "../model/Script.h"
#include "../model/ScriptConfig.h"
#include "../view/ControlBar.h"
#include "../view/MainWindow.h"
#include "../view/TranslationTextPanel.h"

Manager::Manager(MainWindow *pMainWindow) {
#ifdef DEBUG_MANAGER
	{
		std::stringstream log;
		log << "Constructing a new Manager object...";
		LOG_DEBUG(log.str());
	}
#endif
	_pMainWindow = pMainWindow;
	_pOriginalScript = NULL;
	_pTranslationScript = NULL;
	_pConfig = new ScriptConfig;
	_currentPointedTextIndex = -1;

	connect(_pMainWindow, SIGNAL(notifyLoadScriptConfig(const QString &)), this, SLOT(loadConfig(const QString &)));
	connect(_pMainWindow, SIGNAL(notifyLoadOriginalScript(const QString &)), this, SLOT(loadOriginalScript(const QString &)));
	connect(_pMainWindow, SIGNAL(notifyLoadTranslationScript(const QString &)), this, SLOT(loadTranslationScript(const QString &)));
	connect(_pMainWindow, SIGNAL(notifyCloseActionClick()), this, SLOT(unloadScripts()));
	connect(_pMainWindow, SIGNAL(notifySaveActionClick()), this, SLOT(saveTranslation()));

	connect(_pMainWindow->translationTextPanel(), SIGNAL(notifyTranslationTextChg(const QString &)), this, SLOT(modifyTranslationText(const QString &)));
	connect(_pMainWindow->translationTextPanel(), SIGNAL(notifyIsTranslatedButtonToggled(bool)), this, SLOT(modifyTranslated(bool)));
	connect(_pMainWindow->translationTextPanel(), SIGNAL(notifyIsTranslationDoneButtonToggled(bool)), this, SLOT(modifyTranslationDone(bool)));

	connect(_pMainWindow->controlBar(), SIGNAL(notifyNextClicked()), this, SLOT(next()));
	connect(_pMainWindow->controlBar(), SIGNAL(notifyNextUnsureClicked()), this, SLOT(nextUnsure()));
	connect(_pMainWindow->controlBar(), SIGNAL(notifyNextUntranslatedClicked()), this, SLOT(nextUntranslated()));
	connect(_pMainWindow->controlBar(), SIGNAL(notifyPreviousClicked()), this, SLOT(previous()));
	connect(_pMainWindow->controlBar(), SIGNAL(notifyPreviousUnsureClicked()), this, SLOT(previousUnsure()));
	connect(_pMainWindow->controlBar(), SIGNAL(notifyPreviousUntranslatedClicked()), this, SLOT(previousUntranslated()));
	connect(_pMainWindow->controlBar(), SIGNAL(notifyPointedTextNumberChanged(int)), this, SLOT(current(int)));
}

Manager::~Manager() {
#ifdef DEBUG_MANAGER
	{
		std::stringstream log;
		log << "Destroying this Manager...";
		LOG_DEBUG(log.str());
	}
#endif
	unloadScripts();
	delete _pConfig;
}
/**
 * @brief This slots is executed to load config.
 *
 * @param configFileName: xml config file name.
 */
void Manager::loadConfig(const QString & configFileName) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::loadConfig(";
		log << "configFileName: " << configFileName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif

	QFile configFile(configFileName);
	if(!configFile.exists()) {
		{
			std::stringstream err;
			err << "loadConfig() failed! - file '" << configFileName.toStdString() << "' does not exist!";
			LOG_ERROR(err.str());
		}
		return;
	}

	_pConfig->clear();

	if(! XmlConfigReader::getInstance()->loadConfig(&configFile, *_pConfig)) {
		{
			std::stringstream err;
			err << "loadConfig() failed! - error while parsing original xml script file '" << configFileName.toStdString() << "'!";
			LOG_ERROR(err.str());
		}
		return;
	}

	_pMainWindow->setHighlighter(_pConfig);
}

/**
 * @brief This slots is executed to load original script.
 *
 * @param originalScriptFileName: file name of original script to load.
 */
void Manager::loadOriginalScript(const QString & originalScriptFileName) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::loadOriginalScript(";
		log << "originalScriptFileName: " << originalScriptFileName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	QFile originalScriptFile(originalScriptFileName);
	if(!originalScriptFile.exists()) {
		{
			std::stringstream err;
			err << "loadOriginalScript() failed! - file '" << originalScriptFileName.toStdString() << "' does not exist!";
			LOG_ERROR(err.str());
		}
		return;
	}

	if(_pOriginalScript) {
		delete _pOriginalScript;
		_pOriginalScript = NULL;
	}

	_pOriginalScript = new Script(originalScriptFileName);

	if(! XmlScriptReader::getInstance()->loadScript(&originalScriptFile, *_pOriginalScript)) {
		{
			std::stringstream err;
			err << "loadOriginalScript() failed! - error while parsing original xml script file '" << originalScriptFileName.toStdString() << "'!";
			LOG_ERROR(err.str());
		}
		return;
	}
}

/**
 * @brief This slots is executed to load translation script.
 *
 * @param translationScriptFileName: file name of translation script to load.
 */
void Manager::loadTranslationScript(const QString & translationScriptFileName) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::loadTranslationScript(";
		log << "translationScriptFileName: " << translationScriptFileName.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	QFile translationScriptFile(translationScriptFileName);
	if(!translationScriptFile.exists()) {
		{
			std::stringstream err;
			err << "loadTranslationScript() failed - file '" << translationScriptFileName.toStdString() << "' does not exist!";
			LOG_ERROR(err.str());
		}
		return;
	}


	if(_pTranslationScript) {
		delete _pTranslationScript;
		_pTranslationScript = NULL;
	}

	_pTranslationScript = new Script(translationScriptFileName);

	if(! XmlScriptReader::getInstance()->loadScript(&translationScriptFile, *_pTranslationScript)) {
		{
			std::stringstream err;
			err << "loadTranslationScript() failed - error while parsing translation xml script file '" << translationScriptFileName.toStdString() << "'!";
			LOG_ERROR(err.str());
		}
		return;
	}

	if(_pOriginalScript && _pTranslationScript) {
		changePointedText(0);
	}
}

/**
 * @brief This slots is executed to unload both scripts.
 */
void Manager::unloadScripts() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::unloadScripts(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pMainWindow->unloadTextPanels();
	if(_pOriginalScript) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "unloadScripts() - unload original script.";
			LOG_DEBUG(log.str());
		}
#endif
		delete _pOriginalScript;
		_pOriginalScript = NULL;
	}
	if(_pTranslationScript) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "unloadScripts() - unload translation script.";
			LOG_DEBUG(log.str());
		}
#endif
		delete _pTranslationScript;
		_pTranslationScript = NULL;
	}
}

/**
 * @brief Modify the current pointed text.
 *
 * @param text: the new text.
 */
void Manager::modifyTranslationText(const QString & text) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::modifyTranslation(";
		log << "text: " << &text;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(!_pTranslationScript) {
		{
			std::stringstream log;
			log << "modifyTranslation() failed - no translation script has been loaded!";
			LOG_ERROR(log.str());
		}
		return;
	}
	_pTranslationScript->getPointedText(_currentPointedTextIndex)->setText(text);
}

/**
 * @brief Modify the translation status of the current pointed text.
 *
 * @param isTranslationDone: TRUE if this pointed text translation is done, FALSE otherwise.
 */
void Manager::modifyTranslated(bool isTranslated) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::modifyTranslated(";
		log << "isTranslated: " << (isTranslated ? "TRUE" : "FALSE");
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(!_pTranslationScript) {
		{
			std::stringstream log;
			log << "modifyTranslated() failed - no translation script has been loaded!";
			LOG_ERROR(log.str());
		}
		return;
	}
	_pTranslationScript->getPointedText(_currentPointedTextIndex)->setTranslated(isTranslated);
}

/**
 * @brief Modify the translation status of the current pointed text.
 *
 * @param isTranslationDone: TRUE if this pointed text translation is done, FALSE otherwise.
 */
void Manager::modifyTranslationDone(bool isTranslationDone) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::modifyTranslationDone(";
		log << "isTranslationDone: " << (isTranslationDone ? "TRUE" : "FALSE");
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(!_pTranslationScript) {
		{
			std::stringstream log;
			log << "modifyTranslation() failed - no translation script has been loaded!";
			LOG_ERROR(log.str());
		}
		return;
	}
	_pTranslationScript->getPointedText(_currentPointedTextIndex)->setTranslationDone(isTranslationDone);
}

/**
 * @brief This slots is executed to save the current translated script.
 */
void Manager::saveTranslation() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::saveTranslation()";
		LOG_INFO(log.str());
	}
#endif
	if(!_pTranslationScript) {
		{
			std::stringstream log;
			log << "saveTranslation() failed - no translation script has been loaded!";
			LOG_ERROR(log.str());
		}
		return;
	}
	XmlScriptWriter::getInstance()->writeScript(_pTranslationScript);
}

bool Manager::current(int index) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::current(";
		log << "index: " << index;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(index)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "current() - requested index is invalid!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}
	changePointedText(index);
	return true;
}

/**
 * @brief Request for the next pointed text.
 */
bool Manager::next() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::next()";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(_currentPointedTextIndex + 1)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "next() - current index is the last one!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}
	changePointedText(_currentPointedTextIndex + 1);
	return true;
}

/**
 * @brief Request for the next unsure pointed text.
 */
bool Manager::nextUnsure() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::nextUnsure()";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(_currentPointedTextIndex + 1)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "nextUnsure() - current index is the last one!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	bool found = false;
	int unsureIndex = _currentPointedTextIndex;
	while(!found && unsureIndex < _pTranslationScript->size()) {
		if(!_pTranslationScript->getPointedText(++unsureIndex)->isTranslationDone()) {
			found = true;
		}
	}

	if(!found) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "nextUnsure() - there is no unsure pointed text after " << _currentPointedTextIndex;
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	changePointedText(unsureIndex);

	return true;
}

/**
 * @brief Request for the next untranslated pointed text.
 */
bool Manager::nextUntranslated() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::nextUntranslated()";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(_currentPointedTextIndex + 1)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "nextUntranslated() - current index is the last one!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	bool found = false;
	int untranslatedIndex = _currentPointedTextIndex;
	while(!found && untranslatedIndex < _pTranslationScript->size()) {
		if(!_pTranslationScript->getPointedText(++untranslatedIndex)->isTranslated()) {
			found = true;
		}
	}

	if(!found) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "nextUntranslated() - there is no untranslated pointed text after " << _currentPointedTextIndex;
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	changePointedText(untranslatedIndex);

	return true;
}

/**
 * @brief Request for the previous pointed text.
 */
bool Manager::previous() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::previous()";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(_currentPointedTextIndex - 1)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "next() - current index is the first one!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	changePointedText(_currentPointedTextIndex - 1);
	return true;
}

/**
 * @brief Request for the previous unsure pointed text.
 */
bool Manager::previousUnsure() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::previousUnsure()";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(_currentPointedTextIndex - 1)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "previousUnsure() - current index is the first one!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	bool found = false;
	int unsureIndex = _currentPointedTextIndex;
	while(!found && unsureIndex > 0) {
		if(!_pTranslationScript->getPointedText(--unsureIndex)->isTranslationDone()) {
			found = true;
		}
	}

	if(!found) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "previousUnsure() - there is no unsure pointed text before " << _currentPointedTextIndex;
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	changePointedText(unsureIndex);

	return true;
}

/**
 * @brief Request for the previous untranslated pointed text.
 */
bool Manager::previousUntranslated() {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::previousUntranslated()";
		LOG_INFO(log.str());
	}
#endif
	if(!isAvailable(_currentPointedTextIndex - 1)) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "previousUntranslated() - current index is the first one!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	bool found = false;
	int untranslatedIndex = _currentPointedTextIndex;
	while(!found && untranslatedIndex > 0) {
		if(!_pTranslationScript->getPointedText(--untranslatedIndex)->isTranslated()) {
			found = true;
		}
	}

	if(!found) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "previousUntranslated() - there is no unsure pointed text before " << _currentPointedTextIndex;
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	changePointedText(untranslatedIndex);

	return true;
}

//=============================================================================
// PRIVATE METHODS
//=============================================================================

bool Manager::isAvailable(int index) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::isAvailable(";
		log << "index: " << index;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(!_pOriginalScript || !_pTranslationScript) {
#ifdef DEBUG_MANAGER
		{
			std::stringstream log;
			log << "isAvailable() - no script has still been loaded!";
			LOG_DEBUG(log.str());
		}
#endif
		return false;
	}

	return (index >= 0 && index < _pTranslationScript->size());

}

/**
 * @brief Change the current pointed text.
 *
 * @param index: new pointed text index.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool Manager::changePointedText(int index) {
#ifdef TRACE_MANAGER
	{
		std::stringstream log;
		log << "Manager::changePointedText(";
		log << "index: " << index;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_currentPointedTextIndex = index;
	return _pMainWindow->setPointedText(_currentPointedTextIndex + 1, _pOriginalScript->getPointedText(_currentPointedTextIndex)->text(), _pTranslationScript->getPointedText(_currentPointedTextIndex)->text(), _pTranslationScript->getPointedText(_currentPointedTextIndex)->isTranslated(), _pTranslationScript->getPointedText(_currentPointedTextIndex)->isTranslationDone());
}

#ifndef _WIN_32
log4cxx::LoggerPtr Manager::logger = log4cxx::Logger::getLogger( "controller.Manager" );
#endif
