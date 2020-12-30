#include "mnpch.h"
#include "Minerva/core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


namespace Minerva
{

	Ref<spdlog::logger> Log::s_coreLogger;
	Ref<spdlog::logger> Log::s_clientLogger;

	void Log::init()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.emplace_back(createRef<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(createRef<spdlog::sinks::basic_file_sink_mt>("Minerva.log", true));
		sinks[0]->set_pattern("%^[%T] %n: %v%$");
		sinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_coreLogger = createRef<spdlog::logger>("MINERVA", sinks.begin(), sinks.end());
		spdlog::register_logger(s_coreLogger);
		s_coreLogger->set_level(spdlog::level::trace);
		//s_coreLogger->flush_on(spdlog::level::trace);

		s_clientLogger = createRef<spdlog::logger>("APP", sinks.begin(), sinks.end());
		spdlog::register_logger(s_clientLogger);
		s_clientLogger->set_level(spdlog::level::trace);
		//s_clientLogger->flush_on(spdlog::level::trace);

		MN_CORE_INFO("Log initialised.");
	}

}