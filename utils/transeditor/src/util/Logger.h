/*
 * Logger.h
 *
 * Define the implementation of the logger to communicate with log4cxx library.
 *
 * In each class that implements the logger, define a static logger variable so
 * that it references the Logger.
 * Try to set the logger name like this (it will be easier to configure the
 * logger):
 *
 * Example: for file in <projectRoot/xx/yy.cpp, LOGGER_NAME --> xx.yy.
 *
 * #define LOGGER_NAME	"xx.yy"
 * log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger( LOGGER_NAME ) );
 *
 * To log, use macro:
 * 	LOG_DEBUG("");
 * 	LOG_INFO("");
 * 	LOG_WARN("");
 * 	LOG_ERROR("");
 * 	LOG("FATAL");
 *
 *  Created on: 1 nov. 2010
 *      Author: rid
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

#ifndef _WIN_32
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
#include "log4cxx/logger.h"
#include "log4cxx/xml/domconfigurator.h"
using namespace log4cxx::helpers;


/**
 * Initialize logger configuration, call it once in the main() function.
 *
 * @param configFilePath: full path to the configuration file for log4cxx.
 */
void initializeLoggerConfig(const std::string & configFilePath);

// call log4cxx methods
#define LOG_DEBUG(str) 	LOG4CXX_DEBUG(logger, str)
#define LOG_INFO(str) 	LOG4CXX_INFO(logger, str)
#define LOG_WARN(str) 	LOG4CXX_WARN(logger, str)
#define LOG_ERROR(str) 	LOG4CXX_ERROR(logger, str)
#define LOG_FATAL(str) 	LOG4CXX_FATAL(logger, str)

#define LOG_CONSTRUCTOR(classname)	{ std::stringstream log; \
									log << "Constructing a new " << classname << " object..."; \
									LOG_DEBUG(log.str()); }


#define LOG_DESTROYER(classname)	{ std::stringstream log; \
									log << "Destroying this " << classname << "..."; \
									LOG_DEBUG(log.str()); }
#else
#include <iostream>
#include <sstream>

// call log4cxx methods
#define LOG_DEBUG(str) 	{std::cout << str << std::endl << std::flush;}
#define LOG_INFO(str) 	{std::cout << str << std::endl << std::flush;}
#define LOG_WARN(str) 	{std::cout << str << std::endl << std::flush;}
#define LOG_ERROR(str) 	{std::cout << str << std::endl << std::flush;}
#define LOG_FATAL(str) 	{std::cout << str << std::endl << std::flush;}

#define LOG_CONSTRUCTOR(classname)	{ std::stringstream log; \
									log << "Constructing a new " << classname << " object..."; \
									LOG_DEBUG(log.str()); }


#define LOG_DESTROYER(classname)	{ std::stringstream log; \
									log << "Destroying this " << classname << "..."; \
									LOG_DEBUG(log.str()); }

#endif


#endif /* LOGGER_H_ */
