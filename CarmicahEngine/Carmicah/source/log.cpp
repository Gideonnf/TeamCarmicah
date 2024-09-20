#include "pch.h"
#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Carmicah
{
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%d-%m-%Y %T.%e [%n] %^%l%$ %v");
		// dd - mm - yyyy HH: MM: SS.mmm [LoggerName] LogLevel Message
		sCoreLogger = spdlog::stdout_color_mt("CARMICAH");
		sCoreLogger->set_level(spdlog::level::trace);

		sClientLogger = spdlog::stdout_color_mt("APP");
		sClientLogger->set_level(spdlog::level::trace);
	}

}