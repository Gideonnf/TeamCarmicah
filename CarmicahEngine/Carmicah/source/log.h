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
        static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
        static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }
        static inline const std::vector<std::string>& getLogs() { return logMessages; }
        static void logMessage(const std::string& msg);

    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger;
        static std::shared_ptr<spdlog::logger> sClientLogger;

        static std::vector<std::string> logMessages;
    };


}

#ifdef CM_DEBUG
#define CM_CORE_TRACE(...){ std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str(); ::Carmicah::Log::GetCoreLogger()->trace(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_INFO(...) {  std::string msg = fmt::format(__VA_ARGS__);  ::Carmicah::Log::GetCoreLogger()->info(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_WARN(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetCoreLogger()->warn(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_CORE_ERROR(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetCoreLogger()->error(msg);  ::Carmicah::Log::logMessage(msg);}

#define CM_TRACE(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->trace(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_INFO(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->info(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_WARN(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str(); ::Carmicah::Log::GetClientLogger()->warn(msg);  ::Carmicah::Log::logMessage(msg);}
#define CM_ERROR(...) {  std::ostringstream oss; oss << fmt::format(__VA_ARGS__); std::string msg = oss.str();  ::Carmicah::Log::GetClientLogger()->error(msg);  ::Carmicah::Log::logMessage(msg);}
#endif

//if release build
#ifdef CM_RELEASE
#define CM_CORE_TRACE(...)
#define CM_CORE_INFO(...)
#define CM_CORE_WARN(...)
#define CM_CORE_ERROR(...)
#define CM_TRACE(...)
#define CM_INFO(...)
#define CM_WARN(...)
#define CM_ERROR(...)
#endif



