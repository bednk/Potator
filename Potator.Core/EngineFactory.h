#pragma once
#include "Engine.h"
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 11 // for Systems struct. In case of significant systems number grow I'll think about splitting it into groups
#include <boost/di.hpp>
#include "WindowHandle.h"
#include "Dx11GraphicsDevice.h"
#include "DxShaderCache.h"
#include "Components.h"
#include "Dx11ImGuiSystem.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> 

namespace Potator
{
	class EngineFactory
	{
    public:
        EngineFactory(const LaunchingParams& params)
        {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            GLFWwindow* window = glfwCreateWindow(params.Width, params.Height, params.Title.c_str(), nullptr, nullptr);

            static WindowWrapper winWrapper = { window };
            static WindowHandle handle = { glfwGetWin32Window(window) };

            switch (params.Api)
            {
            case GpuApi::Dx11:
            {
                auto injector = boost::di::make_injector(
                    // components
                    boost::di::bind<ComponentStorage<MeshComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<MaterialComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<TransformComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<VelocityComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<SceneNodeComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<CommandQueueComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<CameraComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<PointLightComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<ScriptComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<ImGuiComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<Components>>().in(boost::di::singleton),
                    // windowing stuff
                    boost::di::bind<WindowWrapper>().to(winWrapper),
                    boost::di::bind<WindowHandle>().to(handle),
                    boost::di::bind<LaunchingParams>().to(params),
                    // gpu api specifics
                    boost::di::bind<IGraphicsDevice>().in(boost::di::singleton).to<Dx11GraphicsDevice>(),
                    boost::di::bind<IShaderCache>().in(boost::di::singleton).to<DxShaderCache>(),
                    boost::di::bind<ImGuiSystem>().in(boost::di::singleton).to<Dx11ImGuiSystem>(),
                    // systems
                    boost::di::bind<SceneGraph>().in(boost::di::singleton),
                    boost::di::bind<MeshRenderer>().in(boost::di::singleton),
                    boost::di::bind<ViewManager>().in(boost::di::singleton),
                    boost::di::bind<MovementSystem>().in(boost::di::singleton),
                    boost::di::bind<FixedStepTracker>().in(boost::di::singleton),
                    boost::di::bind<CommandDispatcher>().in(boost::di::singleton),
                    boost::di::bind<SceneLoader>().in(boost::di::singleton),
                    boost::di::bind<Lighting>().in(boost::di::singleton),
                    boost::di::bind<WindowHandler>().in(boost::di::singleton),
                    boost::di::bind<Systems>().in(boost::di::singleton),
                    boost::di::bind<Engine>().in(boost::di::singleton)
                );

                _systems = injector.create<std::shared_ptr<Systems>>();
                _engine = injector.create<std::shared_ptr<Engine>>();
                _components = injector.create<std::shared_ptr<Components>>();
            }
                break;
            default:
                // I'll do opengl "on monday" I swear
                throw std::invalid_argument("Unsupported gpu api");
            }
        }

        Engine& GetEngine()
        {
            return *_engine;
        }

        Systems& GetSystems()
        {
            return *_systems;
        }

        Components& GetComponents()
        {
            return *_components;
        }

    private:
        std::shared_ptr<Engine> _engine;
        std::shared_ptr<Systems> _systems;
        std::shared_ptr<Components> _components;
	};

}