/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			log.h

 author:		YANG YUJIE 80%
 co-author:		Nicholas Lai 20%

 email:			y.yujie@digipen.edu

 brief:         This file contains the logging system for the engine. It uses the spdlog library to log messages to the console and to a file.
				The logging system is used by the engine and the client. The engine uses the core logger and the client uses the client logger.
				The logging system is disabled in release mode for performance reasons.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include <memory>
#include "CarmicahCore.h"
#include "spdlog/spdlog.h"
#include <sstream>
#include <vector>
#include <fstream>



namespace Carmicah
{
    class Log
    {
    public:
        static void init();
        static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
        static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }
        static inline const std::vector<std::string>& getLogs() { return logMessages; }
        static void logMessage(const std::string& msg);
        static void writeLogsToFile(const std::string& filename);
        static void flushLogsToFile();

        //log assertion
        static void initAssertion();
        static void logCrashMessage(const char* file, int line, const char* function, const char* message);
        static void writeCrashReport();

    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger; 
        static std::shared_ptr<spdlog::logger> sClientLogger; 
        static std::vector<std::string> logMessages;
        static std::ofstream logFile;
        static std::string currentLogFile;
        static const size_t LOG_FLUSH_THRESHOLD = 1000;

        // Assertion
        static std::ofstream crashLogFile;  // Add this declaration
        static std::string currentCrashLogFile;  // Add this declaration
        static std::vector<std::string> crashMessages;  // Add this declaration
        static bool hasCrashed;

    };


}

// Core log macros for logging to the core logger (used by the engine)

#ifndef CM_RELEASE
#define CM_CORE_TRACE(...){ std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str(); ::Carmicah::Log::GetCoreLogger()->trace(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_INFO(...) {  std::string msg = fmt::format(__VA_ARGS__);  ::Carmicah::Log::GetCoreLogger()->info(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_WARN(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetCoreLogger()->warn(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_ERROR(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetCoreLogger()->error(msg);  ::Carmicah::Log::logMessage(msg);}

#define CM_TRACE(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->trace(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_INFO(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->info(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_WARN(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str(); ::Carmicah::Log::GetClientLogger()->warn(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_ERROR(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->error(msg);  ::Carmicah::Log::logMessage(msg);}
#endif

// Release mode logging is disabled for performance reasons

#ifndef CM_DEBUG
#define CM_CORE_TRACE(...)
#define CM_CORE_INFO(...)
#define CM_CORE_WARN(...)
#define CM_CORE_ERROR(...)
#define CM_TRACE(...)
#define CM_INFO(...)
#define CM_WARN(...)
#define CM_ERROR(...)
#endif



// Assertion macros
#ifdef CM_DEBUG 
#define CM_ASSERT(x, ...) { if(!(x)) { CM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define CM_CORE_ASSERT(x, ...) { if(!(x)) { CM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define CM_ASSERT(x, ...)
#define CM_CORE_ASSERT(x, ...)
#endif // CM_DEBUG 
