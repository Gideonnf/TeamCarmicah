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

namespace Carmicah
{
    class Log
    {
    public:
        static void init();
        // Core logger is used by the engine
        static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
        // Client logger is used by the client
        static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }
        // Get all logs
        static inline const std::vector<std::string>& getLogs() { return logMessages; }
        static void logMessage(const std::string& msg);

    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger; // Core logger is used by the engine

        static std::shared_ptr<spdlog::logger> sClientLogger; // Client logger is used by the client

        static std::vector<std::string> logMessages;
    };


}

// Core log macros for logging to the core logger (used by the engine)

#define CM_CORE_TRACE(...){ std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str(); ::Carmicah::Log::GetCoreLogger()->trace(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_INFO(...) {  std::string msg = fmt::format(__VA_ARGS__);  ::Carmicah::Log::GetCoreLogger()->info(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_WARN(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetCoreLogger()->warn(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_ERROR(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetCoreLogger()->error(msg);  ::Carmicah::Log::logMessage(msg);}

#define CM_TRACE(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->trace(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_INFO(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->info(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_WARN(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str(); ::Carmicah::Log::GetClientLogger()->warn(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_ERROR(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->error(msg);  ::Carmicah::Log::logMessage(msg);}


// Release mode logging is disabled for performance reasons
/*
#ifndef CM_DEBUG
#define CM_CORE_TRACE(...)
#define CM_CORE_INFO(...)
#define CM_CORE_WARN(...)
#define CM_CORE_ERROR(...)
#define CM_TRACE(...)
#define CM_INFO(...)
#define CM_WARN(...)
#define CM_ERROR(...)
#endif*/



