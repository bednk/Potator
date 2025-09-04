#pragma once
#include "WindowWrapper.h"
#include "IGraphicsDevice.h"
#include "IShaderCache.h"
#include "Systems.h"



namespace Potator
{
	class Engine
	{
	public:
		Engine(WindowWrapper& mainWindow,
			std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			Systems& systems,
			ComponentStorage<MovementComponent>& movements,
			ComponentStorage<TransformComponent>& transforms);
		SceneLoader& GetLoader();

		void Run();
		~Engine();
	private:
		sf::Window& _mainWindow;
		std::shared_ptr<IGraphicsDevice> _device;
		std::shared_ptr<IShaderCache> _shaderCache;
		Systems& _systems;
		ComponentStorage<MovementComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
	};
}
