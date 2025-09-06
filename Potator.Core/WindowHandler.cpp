#include "WindowHandler.h"
#include "imgui_impl_glfw.h"

Potator::WindowHandler::WindowHandler(WindowWrapper& window) : _window { window }
{
    ImGui_ImplGlfw_InitForOther(_window.Window, true);
}

bool Potator::WindowHandler::Handle()
{
	glfwPollEvents();
    if (glfwWindowShouldClose(_window.Window))
    {
        return false;
    }

    int width, height;
    glfwGetFramebufferSize(_window.Window, &width, &height);
    if (width != _width || height != _height)
    {
        _width = width;
        _height = height;
        WindowResized(width, height);
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard
        || io.WantCaptureMouse)
    {
        return true;
    }

    for (size_t i = 0; i < _inputHandlers.size(); i++)
    {
        _inputHandlers[i]->Handle();
    }

    return true;
}

void Potator::WindowHandler::RegisterInputHandler(std::shared_ptr<IInputHandler> handler)
{
	_inputHandlers.push_back(handler);
}
