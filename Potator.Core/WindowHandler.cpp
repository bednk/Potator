#include "WindowHandler.h"
#include "imgui_impl_glfw.h"

Potator::WindowHandler::WindowHandler(IGlfwWindowProvider& window) : _window { window.GetGlfwWindow() }
{
    ImGui_ImplGlfw_InitForOther(_window, true);
}

bool Potator::WindowHandler::Handle()
{
	glfwPollEvents();
    if (glfwWindowShouldClose(_window))
    {
        return false;
    }

    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
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
