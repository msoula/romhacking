/*
 * ControlBar.h
 *
 *  Created on: 14 nov. 2010
 *      Author: rid
 */

#ifndef CONTROLBAR_H_
#define CONTROLBAR_H_

#include <QtGui/QWidget>

#include "../util/Logger.h"

class QLineEdit;
class QPushButton;

class ControlBar : public QWidget {
	Q_OBJECT
public:
	/**
	 * @brief Get an unique instance of this ControlBar.
	 *
	 * @return an unique instance of this ControlBar.
	 * @warn: check the parent of the returned instance.
	 */
	static ControlBar* instance();

	/**
	 * @brief Destructor of this instance of ControlBar.
	 */
	~ControlBar();

	/**
	 * @brief Set the pointer number.
	 *
	 * @param number: the number.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool setNumber(int number);

Q_SIGNALS:
	/**
	 * @brief This signal is emitted when pointed text number is changed.
	 *
	 * @param index	new pointext text index.
	 */
	void notifyPointedTextNumberChanged(int index);

	/**
	 * @brief This signal is emitted when previous unsure button is clicked.
	 */
	void notifyPreviousUnsureClicked();

	/**
	 * @brief This signal is emitted when previous untranslated button is clicked.
	 */
	void notifyPreviousUntranslatedClicked();

	/**
	 * @brief This signal is emitted when previous button is clicked.
	 */
	void notifyPreviousClicked();

	/**
	 * @brief This signal is emitted when next button is clicked.
	 */
	void notifyNextClicked();

	/**
	 * @brief This signal is emitted when next untranslated button is clicked.
	 */
	void notifyNextUntranslatedClicked();

	/**
	 * @brief This signal is emitted when next unsure button is clicked.
	 */
	void notifyNextUnsureClicked();

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger; /*!< the logger. */
#endif
	static ControlBar* _pInstance;

	QPushButton * _pPreviousUnsureButton;		/*!< previous unsure pointed text button. */
	QPushButton * _pPreviousUntranslatedButton;	/*!< previous untranslated pointed text button. */
	QPushButton * _pPreviousPointerButton;		/*!< previous pointed text button. */
	QPushButton * _pNextPointerButton;			/*!< next pointed text button. */
	QPushButton * _pNextUntranslatedButton;		/*!< next untranslated pointed text button. */
	QPushButton * _pNextUnsureButton;			/*!< next unsure pointed text button. */
	QLineEdit * _pPointerNumberField;			/*!< pointer number field. */

	/**
	 * @brief Constructor of an instance of ControlBar.
	 *
	 * @param pParent: the parent of this ControlBar.
	 */
	ControlBar(QWidget *pParent = NULL);

	/**
	 * @brief Initialize this ControlBar.
	 */
	void initGui();

	/**
	 * @brief Initialize buttons of this ControlBar.
	 */
	void initButtons();

	/**
	 * @brief Initialize field of this ControlBar.
	 */
	void initField();

private Q_SLOTS:
	/**
	 * @brief This slot is triggered when new pointer number has been entered.
	 */
	void pointerNumberEntered();
};

#endif /* CONTROLBAR_H_ */
