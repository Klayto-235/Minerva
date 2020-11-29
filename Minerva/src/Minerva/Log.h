#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Minerva
{

	class  Log
	{
	public:
		static void init();

		static spdlog::logger& getCoreLogger() { return *s_coreLogger; }
		static spdlog::logger& getClientLogger() { return *s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};

}

// Core log macros
#define MN_CORE_TRACE(...)	::Minerva::Log::getCoreLogger().trace(__VA_ARGS__)
#define MN_CORE_INFO(...)	::Minerva::Log::getCoreLogger().info(__VA_ARGS__)
#define MN_CORE_WARN(...)	::Minerva::Log::getCoreLogger().warn(__VA_ARGS__)
#define MN_CORE_ERROR(...)	::Minerva::Log::getCoreLogger().error(__VA_ARGS__)
#define MN_CORE_FATAL(...)	::Minerva::Log::getCoreLogger().critical(__VA_ARGS__)

// Client log macros
#define MN_TRACE(...)		::Minerva::Log::getClientLogger().trace(__VA_ARGS__)
#define MN_INFO(...)		::Minerva::Log::getClientLogger().info(__VA_ARGS__)
#define MN_WARN(...)		::Minerva::Log::getClientLogger().warn(__VA_ARGS__)
#define MN_ERROR(...)		::Minerva::Log::getClientLogger().error(__VA_ARGS__)
#define MN_FATAL(...)		::Minerva::Log::getClientLogger().critical(__VA_ARGS__)