/*
 * ControlBar.cpp
 *
 *  Created on: 14 nov. 2010
 *      Author: rid
 */

#include "ControlBar.h"

#define DEBUG_CONTROLBAR	// Enable debug mode
//#undef DEBUG_CONTROLBAR		// Disable debug mode

#define TRACE_CONTROLBAR	// Enable trace
//#undef TRACE_CONTROLBAR

#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStyle>

#define POINTER_FIELD_FIXED_WIDTH	(33)	// pointer field fixed width

ControlBar* ControlBar::_pInstance = NULL;

/**
 * @brief Get an unique instance of this ControlBar.
 *
 * @return an unique instance of this ControlBar.
 */
ControlBar* ControlBar::instance() {
#ifdef TRACE_CONTROLBAR
	{
		std::stringstream log;
		log << "ControllBar::instance()";
		LOG_INFO(log.str());
	}
#endif
	if(!_pInstance) {
		_pInstance = new ControlBar(0);
	}
	return _pInstance;
}

ControlBar::ControlBar(QWidget *pParent) :QWidget(pParent) {
#ifdef DEBUG_CONTROLBAR
	{
		std::stringstream log;
		log << "Constructing a new ControlBar object..." << std::endl;
		log << "\t\tparent: " << pParent;
		LOG_DEBUG(log.str());
	}
#endif
	_pNextUnsureButton = NULL;
	_pNextUntranslatedButton = NULL;
	_pNextPointerButton = NULL;
	_pPreviousPointerButton = NULL;
	_pPreviousUntranslatedButton = NULL;
	_pPreviousUnsureButton = NULL;
	_pPointerNumberField = NULL;

	initGui();
}

ControlBar::~ControlBar() {
#ifdef DEBUG_CONTROLBAR
	{
		std::stringstream log;
		log << "Destroying this ControlBar...";
		LOG_DEBUG(log.str());
	}
#endif
}

/**
 * @brief Set the pointer number.
 *
 * @param number: the number.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool ControlBar::setNumber(int number) {
	_pPointerNumberField->setText(QString::number(number, 10));
	return true;
}


/**
 * @brief Initialize this ControlBar.
 */
void ControlBar::initGui() {
#ifdef TRACE_CONTROLBAR
	{
		std::stringstream log;
		log << "ControlBar::initGui()";
		LOG_INFO(log.str());
	}
#endif
	initButtons();
	initField();

	QHBoxLayout* pLayout = new QHBoxLayout(this);
	pLayout->addWidget(_pPreviousUnsureButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pPreviousUntranslatedButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pPreviousPointerButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pPointerNumberField, 0, Qt::AlignLeft);
	pLayout->addWidget(_pNextPointerButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pNextUntranslatedButton, 0, Qt::AlignLeft);
	pLayout->addWidget(_pNextUnsureButton, 0, Qt::AlignLeft);
	pLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	setLayout(pLayout);
}

/**
 * @brief Initialize buttons of this ControlBar.
 */
void ControlBar::initButtons() {
#ifdef TRACE_CONTROLBAR
	{
		std::stringstream log;
		log << "ControlBar::initButtons()";
		LOG_INFO(log.str());
	}
#endif
	_pNextUnsureButton = new QPushButton(trUtf8("Next unsure"), this);
	_pNextUntranslatedButton = new QPushButton(trUtf8("Next untranslated"), this);
	_pNextPointerButton = new QPushButton(this);
	_pPreviousPointerButton = new QPushButton(this);
	_pPreviousUnsureButton = new QPushButton(trUtf8("Previous unsure"), this);
	_pPreviousUntranslatedButton = new QPushButton(trUtf8("Previous untranslated"), this);

	QStyle* pStyle =  QApplication::style();
	QIcon rightArrow = pStyle->standardIcon(QStyle::SP_ArrowRight);
	_pNextPointerButton->setIcon(rightArrow);

	QIcon leftArrow = pStyle->standardIcon(QStyle::SP_ArrowLeft);
	_pPreviousPointerButton->setIcon(leftArrow);

	connect(_pPreviousPointerButton, SIGNAL(clicked()), this, SIGNAL(notifyPreviousClicked()));
	connect(_pPreviousUnsureButton, SIGNAL(clicked()), this, SIGNAL(notifyPreviousUnsureClicked()));
	connect(_pPreviousUntranslatedButton, SIGNAL(clicked()), this, SIGNAL(notifyPreviousUntranslatedClicked()));
	connect(_pNextPointerButton, SIGNAL(clicked()), this, SIGNAL(notifyNextClicked()));
	connect(_pNextUnsureButton, SIGNAL(clicked()), this, SIGNAL(notifyNextUnsureClicked()));
	connect(_pNextUntranslatedButton, SIGNAL(clicked()), this, SIGNAL(notifyNextUntranslatedClicked()));
}

/**
 * @brief Initialize field of this ControlBar.
 */
void ControlBar::initField() {
#ifdef TRACE_CONTROLBAR
	{
		std::stringstream log;
		log << "ControlBar::initField()";
		LOG_INFO(log.str());
	}
#endif
    _pPointerNumberField = new QLineEdit(this);
    _pPointerNumberField->setFixedWidth(POINTER_FIELD_FIXED_WIDTH);
    connect(_pPointerNumberField, SIGNAL(editingFinished()), this, SLOT(pointerNumberEntered()));
}

void ControlBar::pointerNumberEntered() {
	bool ok;
	int newIndex;
	if(0 == (newIndex = _pPointerNumberField->text().toInt(&ok, 10)) && !ok) {
		std::stringstream err;
		err << "pointerNumberEntered() failed - cannot convert '" << _pPointerNumberField->text().toStdString() << "' to integer.";
		LOG_ERROR(err.str());
		return;
	}
	if(newIndex < 0) {
		std::stringstream err;
		err << "pointerNumberEntered() failed - an invalid value (" << newIndex << ") has been entered.";
		LOG_ERROR(err.str());
		return;
	}

	Q_EMIT(notifyPointedTextNumberChanged(newIndex - 1));
}

#ifndef _WIN_32
log4cxx::LoggerPtr ControlBar::logger = log4cxx::Logger::getLogger( "view.ControlBar" );
#endif
