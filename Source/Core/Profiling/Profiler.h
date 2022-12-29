#pragma once
#include <chrono>
#include <string>
#include <vector>

#include "Core\Base\Singleton.h"
#include "Core\Log\Logger.h"

namespace SDBX
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using Clock = std::chrono::high_resolution_clock;

	class Profiler final : public Singleton<Profiler>
	{
	public:
		struct TimerHandle
		{
			size_t hash;

			explicit TimerHandle(size_t hash) : hash{ hash } {};
			TimerHandle(const TimerHandle& other) = delete;
			TimerHandle(TimerHandle&& other) noexcept : hash{ other.hash } { other.hash = 0; }
			TimerHandle& operator=(const TimerHandle& other) = delete;
			TimerHandle& operator=(TimerHandle&& other) noexcept { hash = other.hash; other.hash = 0; return *this; }

			bool operator==(const TimerHandle& other) const { return hash == other.hash; }
			bool operator!=(const TimerHandle& other) const { return !(*this == other); }

			~TimerHandle() { SDBX::Profiler::GetInstance().StopTimer(hash); }
		};

		~Profiler() override = default;
		Profiler(const Profiler& other) = delete;
		Profiler(Profiler&& other) noexcept = delete;
		Profiler& operator=(const Profiler& other) = delete;
		Profiler& operator=(Profiler&& other) noexcept = delete;

		TimerHandle StartScopedTimer(const std::string& fileName, const std::string& fncName, uint32_t frameCount = 1);
		size_t StartTimer(const std::string& fileName, const std::string& fncName, uint32_t frameCount = 1);
		void StopTimer(size_t hash);
	private:
		friend class Singleton<Profiler>;
		explicit Profiler() = default;

		struct Timer
		{
			TimePoint startPoint;
			double rate;
			size_t hash;
			std::string fileName;
			std::string fncName;
			uint32_t frameCount;
			uint32_t frame;
			double time;

			bool operator==(const Timer& other) const { return hash == other.hash; }
			bool operator!=(const Timer& other) const { return !(*this == other); }

			inline static size_t Hash(const std::string& fileName, const std::string& fncName) { return std::hash<std::string>()(fileName + ":" + fncName); };
		};

		std::vector<Timer> m_Timers;
	};
}

#if defined(SDBX_PROFILING) || defined(_DEBUG) || defined(DEBUG)
	#define BEGIN_TIMER_PROFILLING_N(frameCount) size_t hash_##__FUNCTION__ = SDBX::Profiler::GetInstance().StartTimer(STR1(__FILE__), __func__, frameCount);
	#define BEGIN_TIMER_PROFILLING() BEGIN_TIMER_PROFILLING_N(1) 
	#define END_TIMER_PROFILLING() SDBX::Profiler::GetInstance().StopTimer(hash_##__FUNCTION__);

	#define SCOPED_TIMER_PROFILLING_N(frameCount) SDBX::Profiler::TimerHandle handle_##__FUNCTION__ = SDBX::Profiler::GetInstance().StartScopedTimer(STR1(__FILE__), __func__, frameCount);
	#define SCOPED_TIMER_PROFILLING() SCOPED_TIMER_PROFILLING_N(1) 
#else
	#define BEGIN_TIMER_PROFILLING_N(frameCount)
	#define BEGIN_TIMER_PROFILLING()
	#define END_TIMER_PROFILLING()

	#define SCOPED_TIMER_PROFILLING_N(frameCount)
	#define SCOPED_TIMER_PROFILLING()
#endif