/*
 * TranslationTextPanel.h
 *
 *  Created on: 14 nov. 2010
 *      Author: rid
 */

#ifndef TRANSLATIONTEXTPANEL_H_
#define TRANSLATIONTEXTPANEL_H_

#include "TextPanel.h"

class QCheckBox;
class QPushButton;

class TranslationTextPanel : public TextPanel {
	Q_OBJECT
public:
	/**
	 * @brief Constructor of an instance of TranslationTextPanel.
	 *
	 * @param title: the title of this TextPanel.
	 * @param pParent: the parent widget of this TextPanel.
	 */
	TranslationTextPanel(const QString & title, QWidget *pParent);

	/**
	 * @brief Destructor of this instance of TranslationTextPanel.
	 */
	~TranslationTextPanel();

	/**
	 * @brief Clear the content of this TextPanel.
	 */
	void unloadText();

	/**
	 * @brief Load a text.
	 *
	 * @param text: the text to load.
	 * @param isTranslated: TRUE if text is translated, FALSE otherwise.
	 * @param isTranslationDone: TRUE if text translation is done, FALSE otherwise.
	 * @return TRUE if the operation succeed, FALSE otherwise.
	 */
	bool loadText(const QString & text, bool isTranslated, bool isTranslationDone);

Q_SIGNALS:
	/**
	 * @brief This signal is emitted when translated button changed.
	 */
	void notifyIsTranslatedButtonToggled(bool isTranslated);

	/**
	 * @brief This signal is emitted when translation done button changed.
	 */
	void notifyIsTranslationDoneButtonToggled(bool isTranslationDone);

	/**
	 * @brief This signal is emitted when this TranslationTextPanel content changed.
	 *
	 * @param text: the new text.
	 */
	void notifyTranslationTextChg(const QString & text);

protected:
	bool eventFilter ( QObject * watched, QEvent * event );

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif

	QPushButton* _pResetButton;		/*!< button that resets the script. */
	QCheckBox* _pTranslatedButton;	/*!< button that announces translated. */
	QCheckBox* _pTranslationDoneButton;		/*!< button that announces translation is sure. */

	/**
	 * @brief Initialize this TranslationTextPanel.
	 */
	void initGui();

	/**
	 * @brief Initialize buttons of this TranslationTextPanel.
	 */
	void initButtons();
};

#endif /* TRANSLATIONTEXTPANEL_H_ */
