#pragma once
#include <vector>
#include "IFixedStep.h"
#include <chrono>

namespace Potator
{
	class FixedStepTracker
	{
	public:
		using Clock = std::chrono::steady_clock;
		using Duration = Clock::duration;
		using TimePoint = Clock::time_point;
		FixedStepTracker();
		FixedStepTracker(int tickRate);
		void MarkFrameStart();
		void Subscribe(IFixedStep* subscriber);
		void Update();
	private:
		std::vector<IFixedStep*> _subscribers;
		int _tickRate;
		Clock _clock;
		Duration _fixedStep;
		Duration _accumulator;
		std::optional<TimePoint> _lastTime;
	};
}

