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
		SceneLoader(IGraphicsDevice& device,
			IShaderCache& shaderCache,
			SceneGraph& graph,
			ViewManager& views,
			Components& components);
		void Load(std::filesystem::path path);
	private:
		IGraphicsDevice& _device;
		IShaderCache& _shaderCache;
		SceneGraph& _sceneGraph;
		ViewManager& _views;
		Components& _components;
	};
}