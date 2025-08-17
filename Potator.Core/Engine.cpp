#include "pch.h"
#include "Engine.h"
#include <SFML/Window.hpp>
#include "Dx11GraphicsDevice.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ComponentStorage.h"
#include "TransformComponent.h"
#include "SceneNodeComponent.h"
#include "SceneGraph.h"
#include "EntityRegistry.h"
#include "MeshRenderer.h"


namespace Potator
{
	struct Engine::Impl
	{
		sf::Window MainWindow;
		std::unique_ptr<Dx11GraphicsDevice> Device;
		ComponentStorage<MeshComponent> Meshes;
		ComponentStorage<TransformComponent> Transforms;
		ComponentStorage<SceneNodeComponent> Tree;
		EntityRegistry Registry;
		MeshRenderer Renderer;
		SceneGraph Graph;

		Impl() :
			MainWindow{ sf::VideoMode({ 800, 600 }), "Potator Engine" },
			Device{ std::make_unique<Dx11GraphicsDevice>(MainWindow.getNativeHandle()) },
			Graph{ Transforms, Tree },
			Renderer{ Device.get(), Meshes, Transforms}
		{

		}

		void Run()
		{
			while (MainWindow.isOpen())
			{
				while (const std::optional event = MainWindow.pollEvent())
				{
					if (!event)
					{
						continue;
					}

					if (event->is<sf::Event::Closed>())
					{
						MainWindow.close();
					}
				}
				
				Device->Clear(0, 0, 0, 1);
				Graph.UpdateTransforms();
				Renderer.Render();
				Device->Present();
			}
		}
	};

	Engine::Engine() :
		_impl{ std::make_unique<Impl>() }
	{

	}

	void Engine::Run()
	{
		_impl->Run();
	}

	EntityRegistry& Engine::GetEntityRegistry() const
	{
		return _impl->Registry;
	}

	ComponentStorage<MeshComponent>& Engine::GetMeshes()
	{
		return _impl->Meshes;
	}

	SceneGraph& Engine::GetSceneGraph() const
	{
		return _impl->Graph;
	}

	IGraphicsDevice* Engine::GetGraphicsDevice() const
	{
		return _impl->Device.get();
	}

	Engine::~Engine() = default;
}