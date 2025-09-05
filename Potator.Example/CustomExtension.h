#pragma once
#include "IEngineExtension.h"
#include "Systems.h"
#include "Components.h"

namespace Potator
{
	class CustomExtension : public IEngineExtension
	{
	public:
		CustomExtension(Systems& systems, Components& components);
		void Initialize() override;
		void OnFrameStarted() override;
		void OnBeforeStateUpdated() override;
		void OnBeforeSceneRendered() override;
		void Cleanup() override;
	private:
		void BindCameraController();
		Systems& _systems;
		Components& _components;
	};
}

