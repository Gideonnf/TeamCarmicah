#include "pch.h"
#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Carmicah
{
	std::shared_ptr<spdlog::logger> log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> log::s_ClientLogger;

	void log::init()
	{
		spdlog::set_pattern("%d-%m-%Y %T.%e [%n] %^%l%$ %v");
		// dd - mm - yyyy HH: MM: SS.mmm [LoggerName] LogLevel Message
		s_CoreLogger = spdlog::stdout_color_mt("CARMICAH");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}