#include "Profiler.h"

size_t SDBX::Profiler::StartTimer(const std::string& fileName, const std::string& fncName, uint32_t frameCount)
{
	Timer timer{};
	timer.hash = Timer::Hash(fileName, fncName);

	auto existingHandle = std::find(std::begin(m_Timers), std::end(m_Timers), timer);

	if (existingHandle != std::end(m_Timers))
	{
		existingHandle->startPoint = Clock::now();
	}
	else
	{
		timer.fileName = fileName;
		timer.fncName = fncName;
		timer.frameCount = frameCount;
		timer.frame = 0;
		timer.rate = 1.0 / frameCount;
		timer.time = 0.0;
		m_Timers.push_back(timer);
		m_Timers.back().startPoint = Clock::now();
	}

	return timer.hash;
}

SDBX::Profiler::TimerHandle SDBX::Profiler::StartScopedTimer(const std::string& fileName, const std::string& fncName, uint32_t frameCount)
{
	return TimerHandle{ StartTimer(fileName, fncName, frameCount) };
}

void SDBX::Profiler::StopTimer(size_t hash)
{
	TimePoint endPoint{ Clock::now() };

	auto existingHandle = std::find_if(std::begin(m_Timers), std::end(m_Timers), [hash](const Timer& t) { return t.hash == hash; });

	if (existingHandle != std::end(m_Timers))
	{
		double duration{ std::chrono::duration<double, std::ratio<1, 1000>>(endPoint - existingHandle->startPoint).count() };
		existingHandle->time += duration * existingHandle->rate;
		++existingHandle->frame;
		if (existingHandle->frame == existingHandle->frameCount)
		{
			SDBX::Logger::Log<std::string>(SDBX::Logger::LogLevel::INFO_LOG, "Profiling Timer: " + existingHandle->fileName + " : " + existingHandle->fncName + " =====> " + std::to_string(duration) + "ms");
			existingHandle->time = 0.0;
			existingHandle->frame = 0;
		}
	}
}