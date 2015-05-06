/*
 * Configuration.cpp
 *
 *  Created on: 1 nov. 2010
 *      Author: rid
 */

#include "Configuration.h"

#include "util/Logger.h"

#include <stdlib.h>
#include <sstream>

/**
 * Get the installation directory path that is contained in a environment
 * variable.
 * @return the installation directory path.
 */
const std::string getInstallDirPath() {
	char* value = NULL;
	if( NULL == (value = getenv(INSTALL_DIR_PATH)) ) {
		{
//			log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger( MAIN_LOGGER_NAME );
//			std::stringstream log;
//			log << "Environment variable for installation directory path '" << INSTALL_DIR_PATH << "' is not set!";
//			log << "--> using './'";
//			LOG_WARN(log.str());
		}
		return std::string(".");
	}
	return std::string(".");
}
