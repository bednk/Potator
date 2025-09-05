#pragma once
#include "Components.h"
#include "IGraphicsDevice.h"
#include "IShaderCache.h"
#include "SceneGraph.h"
#include "ViewManager.h"
#include <filesystem>

namespace Potator
{
	class SceneLoader
	{
	public:
		SceneLoader(std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			SceneGraph& graph,
			ViewManager& views,
			Components& components);
		void Load(std::filesystem::path path);
	private:
		std::shared_ptr<IGraphicsDevice> _device;
		std::shared_ptr<IShaderCache> _shaderCache;
		SceneGraph& _sceneGraph;
		ViewManager& _views;
		Components& _components;
	};
}