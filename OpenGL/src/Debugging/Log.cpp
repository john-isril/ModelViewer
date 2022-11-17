#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::logger{};

void Log::Init()
{
	spdlog::set_pattern("%^[%T] : %v%$");
	logger = spdlog::stdout_color_mt("Logger");
	logger->set_level(spdlog::level::trace);
}
