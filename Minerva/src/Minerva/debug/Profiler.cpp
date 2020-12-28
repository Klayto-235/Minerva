#include "mnpch.h"
#include "Minerva/debug/Profiler.h"

#include <algorithm>
#include <fstream>

namespace Minerva
{

	volatile bool Profiler::s_running = false;
	Scope<std::thread> Profiler::s_profileThread;
	std::vector<ProfileBuffer*> Profiler::s_buffers;
	std::mutex Profiler::s_buffersMutex, Profiler::s_sessionMutex;

	void Profiler::beginSession(const std::string& filePath)
	{
		std::lock_guard<std::mutex> lock(s_sessionMutex);

		if (s_running)
		{
			s_running = false;
			s_profileThread->join();
		}

		s_running = true;
		s_profileThread = createScope<std::thread>(profileThreadFunction, filePath);
	}

	void Profiler::endSession()
	{
		std::lock_guard<std::mutex> lock(s_sessionMutex);
		if (s_running)
		{
			s_running = false;
			s_profileThread->join();
		}
	}

	Profiler::Profiler()
	{
		std::lock_guard<std::mutex> lock(s_buffersMutex);
		s_buffers.push_back(&m_buffer);
	}

	Profiler::~Profiler()
	{
		std::lock_guard<std::mutex> lock(s_buffersMutex);
		auto it = std::find(s_buffers.begin(), s_buffers.end(), &m_buffer);
		MN_CORE_ASSERT(it != s_buffers.end(), "Profiler::~Profiler: Could not find buffer.");
		s_buffers.erase(it);
	}

	void Profiler::profileThreadFunction(const std::string filePath)
	{
		ProfileTimerResult result{};
		std::ofstream outputStream(filePath);
		MN_CORE_ASSERT(outputStream.is_open(), "Could not open file \"{0}\".", filePath);

		outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		outputStream.flush();

		while (s_running)
		{
			{
				std::lock_guard<std::mutex> lock(s_buffersMutex);

				for (auto& buffer : s_buffers) {
					while (buffer->pop(result))
					{
						std::string name(result.name ? result.name : "");
						std::replace(name.begin(), name.end(), '"', '\'');

						outputStream << ",{";
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
				}
			}

			std::this_thread::yield();
		}

		outputStream << "]}";
		outputStream.flush();

		outputStream.close();
	}

}