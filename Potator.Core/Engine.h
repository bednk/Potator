#pragma once
#include "Window.h"
#include "IGraphicsDevice.h"
#include "IShaderCache.h"
#include "Systems.h"
#include "IEngineExtension.h"



namespace Potator
{
	class Engine
	{
	public:
		Engine(IGraphicsDevice& device, Systems& systems, Components& components);
		void SetExtension(IEngineExtension* extension);
		void Run();
		~Engine();
	private:
		void SetDebugWindow();
		IGraphicsDevice& _device;
		Systems& _systems;
		Components& _components;
		std::optional<IEngineExtension*> _extension;
	};
}
