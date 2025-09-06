#pragma once
#include "ImGuiSystem.h"


namespace Potator
{
	class Dx11ImGuiSystem : public ImGuiSystem
	{
	public:
		Dx11ImGuiSystem(ComponentStorage<ImGuiComponent>& components, IGraphicsDevice& device);
		void NewFrame() override;
		void Update() override;
		void Render() override;
	};
}