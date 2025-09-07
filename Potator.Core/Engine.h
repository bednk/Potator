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
		Engine(IGraphicsDevice& device, Systems& systems);
		void SetExtension(IEngineExtension* extension);
		void Run();
		~Engine();
	private:
		IGraphicsDevice& _device;
		Systems& _systems;
		std::optional<IEngineExtension*> _extension;
	};
}
