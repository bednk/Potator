#pragma once
#include <vector>
#include "IFixedStep.h"
#include "LaunchingParams.h"
#include <chrono>

namespace Potator
{
	class FixedStepTracker
	{
	public:
		using Clock = std::chrono::steady_clock;
		using Duration = Clock::duration;
		using TimePoint = Clock::time_point;
		FixedStepTracker(LaunchingParams params);
		void MarkFrameStart();
		void Subscribe(IFixedStep* subscriber);
		void Update();
	private:
		std::vector<IFixedStep*> _subscribers;
		unsigned int _tickRate;
		Clock _clock;
		Duration _fixedStep;
		Duration _accumulator;
		std::optional<TimePoint> _lastTime;
	};
}

