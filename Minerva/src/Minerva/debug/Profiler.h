#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <cstdint>


namespace Minerva
{

	struct TimerResult
	{
		std::string name;
		long long start, stop;
		size_t threadID;
	};

	class Profiler
	{
	public:
		void beginSession(const std::string& filePath = "profile-results.json")
		{
			m_outputStream.open(filePath);
			writeHeader();
		}

		void endSession()
		{
			writeFooter();
			m_outputStream.close();
			m_resultCount = 0;
		}

		void writeResult(const TimerResult& result)
		{
			if (m_resultCount++ > 0)
				m_outputStream << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_outputStream << "{";
			m_outputStream << "\"cat\":\"function\",";
			m_outputStream << "\"dur\":" << (result.stop - result.start) << ',';
			m_outputStream << "\"name\":\"" << name << "\",";
			m_outputStream << "\"ph\":\"X\",";
			m_outputStream << "\"pid\":0,";
			m_outputStream << "\"tid\":" << result.threadID << ",";
			m_outputStream << "\"ts\":" << result.start;
			m_outputStream << "}";

			m_outputStream.flush();
		}

		void writeHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_outputStream.flush();
		}

		void writeFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		static Profiler& get()
		{
			static Profiler instance;
			return instance;
		}

	private:
		std::ofstream m_outputStream;
		int m_resultCount = 0;
	};

	class ProfilingTimer
	{
	public:
		ProfilingTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_startTimePoint = std::chrono::high_resolution_clock::now();
		}

		~ProfilingTimer()
		{
			if (!m_stopped)
				stop();
		}

		void stop()
		{
			auto endTimePoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>
				(m_startTimePoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>
				(endTimePoint).time_since_epoch().count();

			size_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Profiler::get().writeResult({ m_name, start, end, threadID });

			m_stopped = true;
		}
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
		bool m_stopped;
	};

}

#if defined MN_ENABLE_PROFILING
	#define MN_PROFILE_BEGIN_SESSION(filePath) ::Minerva::Profiler::get().beginSession(filePath)
	#define MN_PROFILE_END_SESSION() ::Minerva::Profiler::get().endSession()
	#define MN_PROFILE_SCOPE(name) ::Minerva::ProfilingTimer timer##__LINE__(name)
	#define MN_PROFILE_FUNCTION() MN_PROFILE_SCOPE(__FUNCTION__)
#else
	#define MN_PROFILE_BEGIN_SESSION(filePath)
	#define MN_PROFILE_END_SESSION()
	#define MN_PROFILE_SCOPE(name)
	#define MN_PROFILE_FUNCTION()
#endif 