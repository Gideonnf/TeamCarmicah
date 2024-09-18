#pragma once
#include <memory>
#include "CarmicahCore.h"
#include "spdlog/spdlog.h"

namespace Carmicah
{
    class Log
    {
    public:
        static void init();
        static inline std::shared_ptr<spdlog::logger>& getCoreLogger() { return sCoreLogger; }
        static inline std::shared_ptr<spdlog::logger>& getClientLogger() { return sClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger;
        static std::shared_ptr<spdlog::logger> sClientLogger;
    };


}

#define CM_CORE_TRACE(...) ::Carmicah::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CM_CORE_INFO(...) ::Carmicah::Log::getCoreLogger()->info(__VA_ARGS__)
#define CM_CORE_WARN(...) ::Carmicah::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CM_CORE_ERROR(...) ::Carmicah::Log::getCoreLogger()->error(__VA_ARGS__)

#define CM_TRACE(...) ::Carmicah::Log::getClientLogger()->trace(__VA_ARGS__)
#define CM_INFO(...) ::Carmicah::Log::getClientLogger()->info(__VA_ARGS__)
#define CM_WARN(...) ::Carmicah::Log::getClientLogger()->warn(__VA_ARGS__)
#define CM_ERROR(...) ::Carmicah::Log::getClientLogger()->error(__VA_ARGS__)


//if distr build
#define CM_CORE_TRACE(...)
#define CM_CORE_INFO(...)
#define CM_CORE_WARN(...)
#define CM_CORE_ERROR(...)
#define CM_TRACE(...)
#define CM_INFO(...)
#define CM_WARN(...)
#define CM_ERROR(...)




