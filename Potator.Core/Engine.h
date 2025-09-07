#pragma once
#include "WindowWrapper.h"
#include "IGraphicsDevice.h"
#include "IShaderCache.h"
#include "Systems.h"
#include "IEngineExtension.h"



namespace Potator
{
	class Engine
	{
	public:
		Engine(WindowWrapper& mainWindow,
			IGraphicsDevice& device,
			Systems& systems);

		void SetExtension(IEngineExtension* extension);
		void Run();
		~Engine();
	private:
		WindowWrapper& _mainWindow;
		IGraphicsDevice& _device;
		Systems& _systems;
		std::optional<IEngineExtension*> _extension;
	};
}
