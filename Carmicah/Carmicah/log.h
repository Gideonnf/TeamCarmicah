#pragma once
#include "Carmicah.h"
#include "pch.h"
#include "Carmicah/external/spdlog/include/spdlog.h"

namespace Carmicah
{        
		class log
		{
			public:
				static void init();
				inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
				inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }

			private:
				static std::shared_ptr<spdlog::logger> s_CoreLogger;
				static std::shared_ptr<spdlog::logger> s_ClientLogger;
		};



}
//Core log macros
#define CM_CORE_TRACE(...) ::Carmicah::log::getCoreLogger()->trace(__VA_ARGS__)
#define CM_CORE_INFO(...) ::Carmicah::log::getCoreLogger()->info(__VA_ARGS__)
#define CM_CORE_WARN(...) ::Carmicah::log::getCoreLogger()->warn(__VA_ARGS__)
#define CM_CORE_ERROR(...) ::Carmicah::log::getCoreLogger()->error(__VA_ARGS__)
#define CM_CORE_FATAL(...) ::Carmicah::log::getCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define CM_TRACE(...) ::Carmicah::log::getClientLogger()->trace(__VA_ARGS__)
#define CM_INFO(...) ::Carmicah::log::getClientLogger()->info(__VA_ARGS__)
#define CM_WARN(...) ::Carmicah::log::getClientLogger()->warn(__VA_ARGS__)
#define CM_ERROR(...) ::Carmicah::log::getClientLogger()->error(__VA_ARGS__)
#define CM_FATAL(...) ::Carmicah::log::getClientLogger()->fatal(__VA_ARGS__)
