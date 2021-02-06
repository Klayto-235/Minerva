#pragma once

#include "Minerva/core/core.h"

#pragma warning(push, 0) // This probably isn't portable?
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace Minerva
{

	class Log
	{
	public:
		static void init();

		static Ref<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		static Ref<spdlog::logger>& getClientLogger() { return s_clientLogger; }
	private:
		static Ref<spdlog::logger> s_coreLogger;
		static Ref<spdlog::logger> s_clientLogger;
	};

}

// Core log macros
#define MN_CORE_TRACE(...)		::Minerva::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MN_CORE_INFO(...)		::Minerva::Log::getCoreLogger()->info(__VA_ARGS__)
#define MN_CORE_WARN(...)		::Minerva::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MN_CORE_ERROR(...)		::Minerva::Log::getCoreLogger()->error(__VA_ARGS__)
#define MN_CORE_CRITICAL(...)	::Minerva::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MN_TRACE(...)		::Minerva::Log::getClientLogger()->trace(__VA_ARGS__)
#define MN_INFO(...)		::Minerva::Log::getClientLogger()->info(__VA_ARGS__)
#define MN_WARN(...)		::Minerva::Log::getClientLogger()->warn(__VA_ARGS__)
#define MN_ERROR(...)		::Minerva::Log::getClientLogger()->error(__VA_ARGS__)
#define MN_CRITICAL(...)	::Minerva::Log::getClientLogger()->critical(__VA_ARGS__)