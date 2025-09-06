#pragma once
#include "IGraphicsDevice.h"
#include "ImGuiComponent.h"
#include "ComponentStorage.h"

namespace Potator
{
	class ImGuiSystem
	{
	public:
		ImGuiSystem(ComponentStorage<ImGuiComponent>& components, IGraphicsDevice& device);
		virtual void NewFrame() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		~ImGuiSystem();
	protected:
		ComponentStorage<ImGuiComponent>& _components;
	};
}