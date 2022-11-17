#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::logger{};

void Log::Init()
{
	spdlog::set_pattern("%^[%T] : %v%$");
	logger = spdlog::stdout_color_mt("Logger");
	logger->set_level(spdlog::level::trace);
}

void Log::LogTrace(const char* msg)
{
	GetLogger()->trace(msg);
}

void Log::LogInfo(const char* msg)
{
	GetLogger()->info(msg);
}

void Log::LogWarn(const char* msg)
{
	GetLogger()->warn(msg);
}

void Log::LogError(const char* msg)
{
	GetLogger()->error(msg);
}

void Log::LogFatal(const char* msg)
{
	GetLogger()->error(msg);
}
