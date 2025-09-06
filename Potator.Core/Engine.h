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
			std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			Systems& systems);

		void SetExtension(IEngineExtension* extension);
		void Run();
		~Engine();
	private:
		WindowWrapper& _mainWindow;
		std::shared_ptr<IGraphicsDevice> _device;
		std::shared_ptr<IShaderCache> _shaderCache;
		Systems& _systems;
		std::optional<IEngineExtension*> _extension;
	};
}
