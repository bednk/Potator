#pragma once

namespace Potator
{
	class IEngineExtension
	{
	public:
		// Called before main game loop.
		virtual void Initialize() = 0;
		// Called at the frame start.
		virtual void OnFrameStarted() = 0;
		// Called after input handling, before state update.
		virtual void OnBeforeStateUpdated() = 0;
		// Called after state update, before rendering.
		virtual void OnBeforeSceneRendered() = 0;
		// Called after main game loop.
		virtual void Cleanup() = 0;

		virtual ~IEngineExtension() = default;
	};
}