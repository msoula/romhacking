/*
 * Logger.cpp
 *
 *  Created on: 1 nov. 2010
 *      Author: rid
 */

#include "Logger.h"

#ifndef _WIN_32
void initializeLoggerConfig(const std::string & configFilePath) {
	log4cxx::xml::DOMConfigurator::configure(configFilePath);
}
#endif
