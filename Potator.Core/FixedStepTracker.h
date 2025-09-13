#pragma once
#include <vector>
#include "IFixedStep.h"
#include "LaunchingParams.h"
#include "FrameClock.h"
#include <chrono>

namespace Potator
{
	class FixedStepTracker
	{
	public:

		FixedStepTracker(LaunchingParams params, FrameClock& clock);
		void Subscribe(IFixedStep* subscriber);
		void Update();
	private:
		std::vector<IFixedStep*> _subscribers;
		unsigned int _tickRate;
		FrameClock& _clock;
		Duration _fixedStep;
		Duration _accumulator;
	};
}

