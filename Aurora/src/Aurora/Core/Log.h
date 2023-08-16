#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
namespace Aurora
{
	class AURORA_API Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger> GetCoreLogger() {
			return s_coreLogger;
		}

		static std::shared_ptr<spdlog::logger> GetClientLogger() {
			return s_clientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#define AURORA_CORE_TRACE(...)     ::Aurora::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AURORA_CORE_INFO(...)      ::Aurora::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AURORA_CORE_WARN(...)      ::Aurora::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AURORA_CORE_ERROR(...)     ::Aurora::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AURORA_CORE_FATAL(...)     ::Aurora::Log::GetCoreLogger()->critical(__VA_ARGS__)


#define AURORA_INFO(...)           ::Aurora::Log::GetClientLogger()->info(__VA_ARGS__)
#define AURORA_TRACE(...)          ::Aurora::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AURORA_WARN(...)           ::Aurora::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AURORA_ERROR(...)          ::Aurora::Log::GetClientLogger()->error(__VA_ARGS__)
#define AURORA_FATAL(...)          ::Aurora::Log::GetClientLogger()->critical(__VA_ARGS__)