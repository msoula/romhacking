/*
 * Script.cpp
 *
 *  Created on: 13 nov. 2010
 *      Author: rid
 */

#define DEBUG_SCRIPT	// Enable debug mode
#undef DEBUG_SCRIPT		// Disable debug mode

#define TRACE_SCRIPT	// Enable trace mode
#undef TRACE_SCRIPT		// Disable trace mode

#include "Script.h"

Script::Script(const QString & fileName) : QObject() {
#ifdef DEBUG_SCRIPT
	{
		std::stringstream log;
		log << "Constructing a new Script object...";
		LOG_DEBUG(log.str());
	}
#endif
	_fileName = fileName;
}

Script::~Script() {
#ifdef DEBUG_SCRIPT
	{
		std::stringstream log;
		log << "Destroying this Script...";
		LOG_DEBUG(log.str());
	}
#endif
	_pointedTexts.clear();
}

/**
 * @brief Get the number of PointedText in this Script.
 *
 * @return the number of PointedText in this Script
 */
int Script::size() const {
#ifdef TRACE_SCRIPT
	{
		std::stringstream log;
		log << "Script::size(";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	return _pointedTexts.size();
}

/**
 * @brief Append a PointedText in this Script.
 *
 * @param pointedText: the PointedText to append.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool Script::append(const PointedText & pointedText) {
#ifdef TRACE_SCRIPT
	{
		std::stringstream log;
		log << "Script::append(";
		log << "pointedText: " << &pointedText;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pointedTexts.append(pointedText);

	return true;
}

/**
 * @brief Insert a PointedText in this Script.
 *
 * @param index: the index where to insert pointedText
 * @param pointedText: the PointedText to insert.
 * @return TRUE if operation succeed, FALSE otherwise.
 */
bool Script::insert(int index, const PointedText & pointedText) {
#ifdef TRACE_SCRIPT
	{
		std::stringstream log;
		log << "Script::insert(";
		log << "index: " << index;
		log << ", pointedText: " << &pointedText;
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	if(index < 0) {
		{
			std::stringstream err;
			err << "Invalid parameter value: index must be a positive number!";
			LOG_ERROR(err.str());
		}
		return false;
	}

	_pointedTexts.insert(index, pointedText);

	return true;
}

/**
 * @brief Get a given pointed text.
 *
 * @param index: the index of the pointed text to return.
 * @return a PointedText or NULL if index is invalid.
 */
PointedText* Script::getPointedText(int index) {
#ifdef TRACE_SCRIPT
	{
		std::stringstream log;
		log << "Script::getPointedText(";
		log << "index: " << index;
		log ")";
		LOG_INFO(log.str());
	}
#endif
	if(index < 0 || index > _pointedTexts.size()) {
		{
			std::stringstream err;
			err << "Invalid index!";
			LOG_ERROR(err.str());
		}
		return NULL;
	}

	return &(_pointedTexts[index]);
}

#ifndef _WIN_32
log4cxx::LoggerPtr Script::logger = log4cxx::Logger::getLogger( "model.Script" );
#endif
