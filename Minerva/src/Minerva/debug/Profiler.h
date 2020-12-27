#pragma once

#include "Minerva/core/core.h"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <atomic>
#include <cstdint>


namespace Minerva
{
	// NOT THREAD SAFE

	struct ProfileTimerResult
	{
		const char* name;
		long long start, stop;
		size_t threadID;
	};

	class ProfileBuffer
	{
	public:
		void push(ProfileTimerResult&& element)
		{
			const uint16_t head = m_head.load(std::memory_order_relaxed);
			if (head == m_tail.load(std::memory_order_relaxed))
			{
				MN_CORE_ERROR("ProfilerBuffer::push: Buffer overflow.");
				MN_CORE_ASSERT(false, "Error.");
			}

			m_buffer[head] = element;
			m_head.store((head + 1) & mc_bufferSizeMask, std::memory_order_release);
		}

		bool pop(ProfileTimerResult& out_element)
		{
			const uint16_t tail = m_tail.load(std::memory_order_relaxed);
			const uint16_t newTail = (tail + 1) & mc_bufferSizeMask;
			const uint16_t head = m_head.load(std::memory_order_acquire);
			if (newTail != head)
			{
				out_element = m_buffer[newTail];
				m_tail.store(newTail, std::memory_order_release);
				return true;
			}
			else
			{
				return false;
			}
		}

	private:
		static constexpr int mc_bufferSizeLog2 = 12; // <= 16
		static constexpr int mc_bufferSizeMask = BIT(mc_bufferSizeLog2) - 1;
		std::atomic_uint16_t m_head = 1, m_tail = 0;
		std::array<ProfileTimerResult, mc_bufferSizeMask + 1> m_buffer{};
	};

	class Profiler
	{
	public:
		Profiler()
		{

		}

		~Profiler()
		{
			endSession();
		}

		void beginSession(const std::string& filePath = "profile-results.json")
		{
			if (m_running)
				endSession();
			m_running = true;
			m_profilerThread = createScope<std::thread>(profileThreadFunction,
				std::ref(m_running), std::ref(m_buffer), filePath);
		}

		void endSession()
		{
			if (m_running)
			{
				m_running = false;
				m_profilerThread->join();
			}
		}

		void writeResult(ProfileTimerResult&& result)
		{
			m_buffer.push(std::move(result));
		}

		static Profiler& get()
		{
			static Profiler instance;
			return instance;
		}

	private:
		static void profileThreadFunction(bool& run, ProfileBuffer& buffer, const std::string filePath)
		{
			ProfileTimerResult result{};
			int resultCount = 0;
			std::ofstream outputStream(filePath);
			MN_CORE_ASSERT(outputStream.is_open(), "Could not open file \"{0}\".", filePath);

			outputStream << "{\"otherData\": {},\"traceEvents\":[";
			outputStream.flush();

			while (run)
			{
				while (buffer.pop(result))
				{
					if (resultCount++ > 0)
						outputStream << ",";

					std::string name(result.name ? result.name : "");
					std::replace(name.begin(), name.end(), '"', '\'');

					outputStream << "{";
					outputStream << "\"cat\":\"function\",";
					outputStream << "\"dur\":" << (result.stop - result.start) << ',';
					outputStream << "\"name\":\"" << name << "\",";
					outputStream << "\"ph\":\"X\",";
					outputStream << "\"pid\":0,";
					outputStream << "\"tid\":" << result.threadID << ",";
					outputStream << "\"ts\":" << result.start;
					outputStream << "}";

					outputStream.flush();
				}

				std::this_thread::yield();
			}

			outputStream << "]}";
			outputStream.flush();

			outputStream.close();
		}

		bool m_running = false;
		ProfileBuffer m_buffer;
		Scope<std::thread> m_profilerThread;
	};

	class ProfileTimer
	{
	public:
		ProfileTimer(const char* name)
			: m_name(name)
		{
			m_startTimePoint = std::chrono::high_resolution_clock::now();
		}

		~ProfileTimer()
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
		bool m_stopped = false;
	};

}

#if defined MN_ENABLE_PROFILING
	#define MN_PROFILE_BEGIN_SESSION(filePath) ::Minerva::Profiler::get().beginSession(filePath)
	#define MN_PROFILE_END_SESSION() ::Minerva::Profiler::get().endSession()
	#define MN_PROFILE_SCOPE(name) ::Minerva::ProfileTimer timer##__LINE__(name)
	#define MN_PROFILE_FUNCTION() MN_PROFILE_SCOPE(__FUNCTION__)
#else
	#define MN_PROFILE_BEGIN_SESSION(filePath)
	#define MN_PROFILE_END_SESSION()
	#define MN_PROFILE_SCOPE(name)
	#define MN_PROFILE_FUNCTION()
#endif 