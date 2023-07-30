#include "pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Aurora {
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_coreLogger = spdlog::stdout_color_mt("AURORA");
		s_coreLogger->set_level(spdlog::level::level_enum::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::level_enum::trace);
	}
}