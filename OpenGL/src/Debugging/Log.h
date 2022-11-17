#pragma once

#include <spdlog/spdlog.h>
#include <memory>

class Log
{
public:
	static void Init();
	static void LogTrace(const char *msg);
	static void LogInfo(const char* msg);
	static void LogWarn(const char* msg);
	static void LogError(const char* msg);
	static void LogFatal(const char* msg);
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; }

private:
	static std::shared_ptr<spdlog::logger> logger;

};