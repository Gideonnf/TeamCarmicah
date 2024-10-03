/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			log.cpp

 author:		YANG YUJIE 80%
 co-author:		Nicholas Lai 20%

 email:			y.yujie@digipen.edu

 brief:         This file contains the implementation of the log class. 

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Carmicah
{
	// Initialize the static variables
	// The static variables are shared pointers to the logger objects
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;
	std::vector<std::string> Log::logMessages;

	// Initialize the logger
	void Log::init()
	{
		spdlog::set_pattern("%d-%m-%Y %T.%e [%n] %^%l%$ %v");
		// dd - mm - yyyy HH: MM: SS.mmm [LoggerName] LogLevel Message
		sCoreLogger = spdlog::stdout_color_mt("CARMICAH");// Create a console logger with the name "CARMICAH"
		// Set the log level to trace
		sCoreLogger->set_level(spdlog::level::trace);
		// Create a console logger with the name "APP"
		sClientLogger = spdlog::stdout_color_mt("APP");
		sClientLogger->set_level(spdlog::level::trace);
	}
	// Get the core logger object 
	void Log::logMessage(const std::string& msg)
	{
		logMessages.push_back(msg);
	}
}