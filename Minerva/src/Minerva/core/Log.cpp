#include "mnpch.h"
#include "Minerva/core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace Minerva
{

	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_coreLogger = spdlog::stdout_color_mt("MINERVA");
		s_coreLogger->set_level(spdlog::level::trace);
		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);

		MN_CORE_INFO("Log initialised.");
	}

}