#include "Dx11ImGuiSystem.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_glfw.h"


Potator::Dx11ImGuiSystem::Dx11ImGuiSystem(ComponentStorage<ImGuiComponent>& components, IGraphicsDevice& device) :
	ImGuiSystem { components, device }
{

}

void Potator::Dx11ImGuiSystem::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Potator::Dx11ImGuiSystem::Update()
{
	auto& componetns = _components.GetComponents();
	for (size_t i = 0; i < componetns.size(); i++)
	{
		componetns[i].Draw();
	}
}

void Potator::Dx11ImGuiSystem::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}
