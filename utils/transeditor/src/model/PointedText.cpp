/*
 * PointedText.cpp
 *
 *  Created on: 13 nov. 2010
 *      Author: rid
 */

#define DEBUG_POINTEDTEXT	// Enable debug mode
#undef DEBUG_POINTEDTEXT	// Disable debug mode

#define TRACE_POINTEDTEXT	// Enable trace mode
#undef TRACE_POINTEDTEXT	// Disable trace mode

#include "PointedText.h"

PointedText::PointedText() : QObject() {
#ifdef DEBUG_POINTEDTEXT
	{
		std::stringstream log;
		log << "Constructing a new PointedText object...";
		LOG_DEBUG(log.str());
	}
#endif
	_pointedText = QString("");
	_isTranslationDone = false;
	_isTranslated = false;
}

/**
 * @brief Copy constructor of an instance of PointedText.
 *
 * @param pointedText: the PointedText to copy.
 */
PointedText::PointedText(const PointedText & pointedText) : QObject() {
#ifdef DEBUG_POINTEDTEXT
	{
		std::stringstream log;
		log << "Constructing a new PointedText..." << std::endl;
		log << "\t\tpointedText:" << &pointedText;
		LOG_DEBUG(log.str());
	}
#endif
	_pointedText = pointedText._pointedText;
	_cpu_address = pointedText._cpu_address;
	_isTranslationDone = pointedText._isTranslationDone;
	_isTranslated = pointedText._isTranslated;

}

PointedText::~PointedText() {
#ifdef DEBUG_POINTEDTEXT
	{
		std::stringstream log;
		log << "Destroying this PointedText...";
		LOG_DEBUG(log.str());
	}
#endif
}

PointedText& PointedText::operator=(const PointedText& pointedText) {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::operator=(";
		log << "pointedText: " << &pointedText;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pointedText = pointedText._pointedText;
	_cpu_address = pointedText._cpu_address;
	_isTranslationDone = pointedText._isTranslationDone;
	_isTranslated = pointedText._isTranslated;
	return *this;
}

/**
 * @brief Get the text of this PointedText.
 *
 * @return the pointed text.
 */
QString PointedText::text() const {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::text(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	return _pointedText;
}

/**
 * @brief Set the text of this PointedText.
 *
 * @param text: the new text.
 */
void PointedText::setText(const QString & text) {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::setText(";
		log << "text: " << text.toStdString();
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(QString::compare(_pointedText, text, Qt::CaseSensitive) == 0) {
		return;
	}

	_pointedText = text;
}

/**
 * @brief Get if this PointedText is translated or not.
 *
 * @return TRUE if translated, FALSE otherwise.
 */
bool PointedText::isTranslated() const {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::isTranslated(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	return _isTranslated;
}

/**
 * @brief Set this PointedText translated or not.
 *
 * @param isTranslated: TRUE to set translated, FALSE otherwise.
 */
void PointedText::setTranslated(bool isTranslated) {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::setTranslated(";
		log << "isTranslated: " << (isTranslated ? "TRUE" : "FALSE");
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(_isTranslated == isTranslated) {
		return;
	}

	_isTranslated = isTranslated;
}

/**
 * @brief Get if the translation of this PointedText is done or not.
 *
 * @return TRUE if the translation is done, FALSE otherwise.
 */
bool PointedText::isTranslationDone() const {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::isTranslationDone(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	return _isTranslationDone;
}

/**
 * @brief Set the translation of this PointedText done or not.
 *
 * @param isTranslationDone: TRUE to set translation done, FALSE otherwise.
 */
void PointedText::setTranslationDone(bool isTranslationDone) {
#ifdef TRACE_POINTEDTEXT
	{
		std::stringstream log;
		log << "PointedText::setTranslationDone(";
		log << "isTranslationDone:" << (isTranslationDone ? "TRUE" :  "FALSE");
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(_isTranslationDone == isTranslationDone) {
		return;
	}

	_isTranslationDone = isTranslationDone;
}

QString PointedText::getCpuAddress() {
	return _cpu_address;
}

void PointedText::setCpuAddress(const QString& cpu_address) {
	_cpu_address = cpu_address;
}

#ifndef _WIN_32
log4cxx::LoggerPtr PointedText::logger = log4cxx::Logger::getLogger( "model.PointedText" );
#endif
