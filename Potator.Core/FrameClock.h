#pragma once
#include <chrono>

using Clock = std::chrono::steady_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;

namespace Potator
{
	class FrameClock
	{
	public:
		void Update();
		Duration GetFrameTime();
	private:
		Clock _clock;
		Duration _frameTime;
		std::optional<TimePoint> _lastTime;
	};
}