#pragma once

#include "core.h"
#include "spdlog/spdlog.h"

namespace Minerva {

	class MINERVA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};

}

// Core log macros
#define MN_CORE_TRACE(...)	::Minerva::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MN_CORE_INFO(...)	::Minerva::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MN_CORE_WARN(...)	::Minerva::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MN_CORE_ERROR(...)	::Minerva::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MN_CORE_FATAL(...)	::Minerva::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define MN_TRACE(...)		::Minerva::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MN_INFO(...)		::Minerva::Log::GetClientLogger()->info(__VA_ARGS__)
#define MN_WARN(...)		::Minerva::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MN_ERROR(...)		::Minerva::Log::GetClientLogger()->error(__VA_ARGS__)
#define MN_FATAL(...)		::Minerva::Log::GetClientLogger()->fatal(__VA_ARGS__)