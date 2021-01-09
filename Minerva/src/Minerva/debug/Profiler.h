#pragma once

#include "Minerva/core/core.h"

#include <string>
#include <chrono>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>


namespace Minerva
{

	struct ProfileTimerResult
	{
		long long start, stop;
		const char* name;
		unsigned threadID;
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
			m_head.store((head + 1) & sc_bufferSizeMask, std::memory_order_release);
		}

		bool pop(ProfileTimerResult& out_element)
		{
			const uint16_t tail = m_tail.load(std::memory_order_relaxed);
			const uint16_t newTail = (tail + 1) & sc_bufferSizeMask;
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
		static constexpr int sc_bufferSizeLog2 = 12; // <= 16
		static constexpr int sc_bufferSizeMask = BIT(sc_bufferSizeLog2) - 1;
		std::atomic_uint16_t m_head = 1, m_tail = 0;
		std::array<ProfileTimerResult, sc_bufferSizeMask + 1> m_buffer{};
	};

	class Profiler
	{
	public:
		~Profiler();

		void writeResult(const char* name, long long start, long long stop)
		{
			m_buffer.push({ start, stop, name, m_threadID });
		}

		static void beginSession(const std::string& filePath);
		static void endSession();

		static Profiler& get()
		{
			thread_local Profiler instance;
			return instance;
		}

	private:
		Profiler();

		static void int_endSession();
		static void profileThreadFunction(const std::string filePath);

		unsigned m_threadID;
		ProfileBuffer m_buffer;

		static volatile bool s_running;
		static std::string s_filePath;
		static Scope<std::thread> s_profileThread;
		static std::vector<ProfileBuffer*> s_buffers;
		static std::mutex s_buffersMutex, s_sessionMutex;
		static unsigned s_threadIDCounter;
	};

	// WARNING: currently, ProfileTimer name pointer is used at the end of the profile session.
	// It must not be invalidated until then.
	class ProfileTimer
	{
	public:
		ProfileTimer(const char* name)
			: m_name(name)
		{
			m_startTimePoint = std::chrono::steady_clock::now();
		}

		~ProfileTimer()
		{
			if (!m_stopped)
				stop();
		}

		void stop()
		{
			auto stopTimePoint = std::chrono::steady_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>
				(m_startTimePoint).time_since_epoch().count();
			long long stop = std::chrono::time_point_cast<std::chrono::microseconds>
				(stopTimePoint).time_since_epoch().count();

			Profiler::get().writeResult(m_name, start, stop);

			m_stopped = true;
		}

	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock> m_startTimePoint;
		bool m_stopped = false;
	};

}

//#define MN_ENABLE_PROFILING

#if defined MN_ENABLE_PROFILING
	#define MN_PROFILE_BEGIN_SESSION(filePath) ::Minerva::Profiler::beginSession(filePath)
	#define MN_PROFILE_END_SESSION() ::Minerva::Profiler::endSession()
	#define MN_PROFILE_SCOPE(name) ::Minerva::ProfileTimer UNIQUE(timer)(name)
	#define MN_PROFILE_FUNCTION() MN_PROFILE_SCOPE(__FUNCTION__)
#else
	#define MN_PROFILE_BEGIN_SESSION(filePath)
	#define MN_PROFILE_END_SESSION()
	#define MN_PROFILE_SCOPE(name)
	#define MN_PROFILE_FUNCTION()
#endif 