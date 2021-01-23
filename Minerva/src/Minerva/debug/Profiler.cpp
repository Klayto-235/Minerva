#include "mnpch.h"
#include "Minerva/debug/Profiler.h"

#include <algorithm>
#include <fstream>
#include <cstring>
#include <cstdio>

namespace Minerva
{
	volatile bool Profiler::s_running = false;
	std::string Profiler::s_filePath;
	Scope<std::thread> Profiler::s_profileThread;
	std::vector<ProfileBuffer*> Profiler::s_buffers;
	std::mutex Profiler::s_buffersMutex, Profiler::s_sessionMutex;
	unsigned Profiler::s_threadIDCounter = 0;

	void Profiler::beginSession(const std::string& filePath)
	{
		std::lock_guard<std::mutex> lock(s_sessionMutex);

		impl_endSession();

		s_filePath = filePath;
		s_running = true;
		s_profileThread = createScope<std::thread>(profileThreadFunction, filePath);
	}

	void Profiler::endSession()
	{
		std::lock_guard<std::mutex> lock(s_sessionMutex);
		impl_endSession();
	}

	Profiler::Profiler()
	{
		std::lock_guard<std::mutex> lock(s_buffersMutex);
		s_buffers.push_back(&m_buffer);
		// Also making use of the mutex:
		m_threadID = s_threadIDCounter;
		++s_threadIDCounter; // Best not chew up all 2^32 IDs!
	}

	void Profiler::impl_endSession()
	{
		if (s_running)
		{
			s_running = false;
			s_profileThread->join();

			// We're holding a mutex but it shouldn't be a problem.

			const std::string binaryFilePath = s_filePath + ".bin";
			const std::string jsonFilePath = s_filePath + ".json";
			std::ifstream inputStream(s_filePath + ".bin", std::ios::binary);
			std::ofstream outputStream(s_filePath + ".json");
			MN_CORE_ASSERT(inputStream.is_open(), "Could not open file \"{0}\".", binaryFilePath);
			MN_CORE_ASSERT(outputStream.is_open(), "Could not open file \"{0}\".", jsonFilePath);

			inputStream.seekg(0, std::ios::end);
			size_t inputSize = inputStream.tellg();
			MN_CORE_ASSERT(inputSize != -1, "Could not read from file \"{0}\".", binaryFilePath);
			MN_CORE_ASSERT(inputSize % sizeof(ProfileTimerResult) == 0, "Unexpected size of file \"{0}\".", binaryFilePath);
			inputStream.seekg(0, std::ios::beg);

			outputStream << "{\"otherData\": {},\"traceEvents\":[{}";

			constexpr size_t bufferSize = sizeof(ProfileTimerResult) << 20;
			std::string buffer;
			while (inputSize > 0)
			{
				const size_t readSize = inputSize > bufferSize ? bufferSize : inputSize;
				inputSize -= readSize;

				buffer.resize(readSize);
				inputStream.read(buffer.data(), readSize);
				MN_CORE_ASSERT(readSize == inputStream.gcount(),
					"Could not read contents of file \"{0}\".", binaryFilePath);

				// Interpret the buffer as an array of ProfileTimerResults.
				// WARNING: setting const char* from file!
				for (ProfileTimerResult* result = reinterpret_cast<ProfileTimerResult*>(buffer.data());
					result < reinterpret_cast<ProfileTimerResult*>(&buffer.back()); ++result)
				{
					outputStream << ",{";
					outputStream << "\"cat\":\"function\",";
					outputStream << "\"dur\":" << (result->stop - result->start) << ',';
					outputStream << "\"name\":\"" << result->name << "\",";
					outputStream << "\"ph\":\"X\",";
					outputStream << "\"pid\":0,";
					outputStream << "\"tid\":" << result->threadID << ",";
					outputStream << "\"ts\":" << result->start;
					outputStream << "}";
				}
			}

			outputStream << "]}";

			inputStream.close();
			outputStream.close();
			std::remove(binaryFilePath.c_str());
		}
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
		bool exit = false;
		ProfileTimerResult result{};
		std::ofstream outputStream(filePath + ".bin", std::ios::binary);
		MN_CORE_ASSERT(outputStream.is_open(), "Could not open file \"{0}\".", filePath + ".bin");

		while (true)
		{
			if (!s_running)
				exit = true;

			{
				bool noResults = true;
				std::lock_guard<std::mutex> lock(s_buffersMutex);
				do
				{
					noResults = true;
					for (auto& buffer : s_buffers) {
						while (buffer->pop(result))
						{
							noResults = false;
							// WARNING: writing const char* to file!
							outputStream.write(reinterpret_cast<const char*>(&result), sizeof(ProfileTimerResult));
						}
					}
				}
				while (!noResults);
			}

			if (exit)
				break;

			// minimum sleep time on Windows seems to be ~2 ms
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		outputStream.close();
	}

}