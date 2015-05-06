/*
 * Configuration.h
 *
 *  Created on: 1 nov. 2010
 *      Author: rid
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

//==============================================================================
//
// Log configuration
//
//==============================================================================
#define MAIN_LOGGER_NAME	"main"
#define LOG4CXX_CONFIG_PATH	"log4j.xml"		//path to the configuration file for log4cxx

//==============================================================================
// SOFT VERSION
// <SOFT_VERSION>.<SOFT_REVISION>.<SOFT_MINOR_VERSION>
//==============================================================================
#define SOFT_VERSION	(0)
#define SOFT_REVISION	(0)
#define SOFT_MINOR_VERSION	(0)
#define SOFT_COMPILATION_DATE	""

//==============================================================================
//
// Environment
//
//==============================================================================
#define	INSTALL_DIR_PATH	"XSE_INSTALL_DIR"

/**
 * Get the installation directory path that is contained in a environment
 * variable.
 * @return the installation directory path.
 */
const std::string getInstallDirPath();

#endif /* CONFIGURATION_H_ */
