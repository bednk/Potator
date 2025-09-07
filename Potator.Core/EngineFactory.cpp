#include "EngineFactory.h"
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 11 // for Systems struct. In case of significant systems number grow I'll think about splitting it into groups
#include <boost/di.hpp>
#include "Window.h"
#include "Dx11GraphicsDevice.h"
#include "DxShaderCache.h"
#include "Dx11ImGuiSystem.h"

Potator::EngineFactory::EngineFactory(const LaunchingParams& params)
{
    switch (params.Api)
    {
    case GpuApi::Dx11:
    {
        auto injector = boost::di::make_injector(
            // windowing stuff
            boost::di::bind<LaunchingParams>().to(params),
            boost::di::bind<IHwndProvider>().in(boost::di::singleton).to<Window>(),
            boost::di::bind<IGlfwWindowProvider>().in(boost::di::singleton).to<Window>(),
            // gpu api specifics
            boost::di::bind<IGraphicsDevice>().in(boost::di::singleton).to<Dx11GraphicsDevice>(),
            boost::di::bind<IShaderCache>().in(boost::di::singleton).to<DxShaderCache>(),
            boost::di::bind<ImGuiSystem>().in(boost::di::singleton).to<Dx11ImGuiSystem>(),
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
