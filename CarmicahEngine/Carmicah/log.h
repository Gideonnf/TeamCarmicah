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
        static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
        static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> sCoreLogger;
        static std::shared_ptr<spdlog::logger> sClientLogger;
    };


}

#ifdef CM_DEBUG
#define CM_CORE_TRACE(...) ::Carmicah::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CM_CORE_INFO(...) ::Carmicah::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CM_CORE_WARN(...) ::Carmicah::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CM_CORE_ERROR(...) ::Carmicah::Log::GetCoreLogger()->error(__VA_ARGS__)

#define CM_TRACE(...) ::Carmicah::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CM_INFO(...) ::Carmicah::Log::GetClientLogger()->info(__VA_ARGS__)
#define CM_WARN(...) ::Carmicah::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CM_ERROR(...) ::Carmicah::Log::GetClientLogger()->error(__VA_ARGS__)
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



