#include "ImGuiSystem.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_dx11.h"

Potator::ImGuiSystem::ImGuiSystem(ComponentStorage<ImGuiComponent>& components, IGraphicsDevice& device) :
	_components { components }
{
	device.InitImGuiContext();
}

Potator::ImGuiSystem::~ImGuiSystem()
{
	if (ImGui::GetCurrentContext == nullptr)
	{
		return;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}