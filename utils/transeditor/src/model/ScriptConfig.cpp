/*
 * ScriptConfig.cpp
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#include "ScriptConfig.h"

#define DEBUG_SCRIPTCONFIG
#undef DEBUG_SCRIPTCONFIG

#define TRACE_SCRIPTCONFIG
//#undef TRACE_SCRIPTCONFIG

/**
 * @brief Constructor of an instance of ScriptConfig.
 */
ScriptConfig::ScriptConfig() {
#ifdef DEBUG_SCRIPTCONFIG
	{
		std::stringstream log;
		log << "Constructing a new ScriptConfig object...";
		LOG_DEBUG(log.str());
	}
#endif
	_maximumLineSize = 0;
	_pOpcodes = new QList<T_OPCODE_DATA>;
}

/**
 * @brief Destructor of this instance of ScriptConfig.
 */
ScriptConfig::~ScriptConfig() {
#ifdef DEBUG_SCRIPTCONFIG
	{
		std::stringstream log;
		log << "Destroying this ScriptConfig...";
		LOG_DEBUG(log.str());
	}
#endif
	clear();
	delete _pOpcodes;
}

/**
 * @brief Append a new opcode data.
 *
 * @param opcodeData: the new opcode data.
 */
void ScriptConfig::append(const T_OPCODE_DATA & opcodeData) {
#ifdef TRACE_SCRIPTCONFIG
	{
		std::stringstream log;
		log << "ScriptConfig::append(";
		log << "opcodeData: [" << opcodeData.regexp.pattern().toStdString() << ":" << opcodeData.value << "]";
		log << ")";
		LOG_INFO(log.str());
	}
#endif
	_pOpcodes->append(opcodeData);
}

/**
 * @brief Clear this script config.
 */
void ScriptConfig::clear() {
#ifdef TRACE_SCRIPTCONFIG
	{
		std::stringstream log;
		log << "ScriptConfig::clear()";
		LOG_INFO(log.str());
	}
#endif
	_pOpcodes->clear();
	_maximumLineSize = 0;
}

/**
 * @brief Get this ScriptConfig as a string.
 *
 * @param s: the stream.
 * @param p: the config.
 * @return a string that contains representation of this ScriptConfig.
 */
std::ostream& operator<<(std::ostream& os, const ScriptConfig & config) {
	os << "maximum line size: " << config._maximumLineSize << std::endl;
	std::stringstream str;
	QList<T_OPCODE_DATA>::iterator it;
	for(it = config._pOpcodes->begin(); it < config._pOpcodes->end(); it++) {
		os << "[" << (*it).regexp.pattern().toStdString() << ":" << (*it).value << "]" << std::endl;
	}
	os << str.str();
	return os;
}

#ifndef _WIN_32
log4cxx::LoggerPtr ScriptConfig::logger = log4cxx::Logger::getLogger( "model.ScriptConfig" );
#endif
