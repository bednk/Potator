#pragma once
#include "Engine.h"
#include <boost/di.hpp>
#include "WindowHandle.h"
#include "Dx11GraphicsDevice.h"
#include "DxShaderCache.h"

namespace Potator
{
	static class EngineFactory
	{
    public:
		static Engine& GetEngine(const LaunchingParams& params)
		{
            static WindowWrapper winWrapper = { { sf::VideoMode{ {params.Width, params.Height} }, params.Title } };
            static WindowHandle handle = { winWrapper.Window.getNativeHandle() };

            switch (params.Api)
            {
            case GpuApi::Dx11:
                auto commonBindings = boost::di::make_injector(
                    boost::di::bind<ComponentStorage<MeshComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<MaterialComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<TransformComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<MovementComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<SceneNodeComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<CommandQueueComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<CameraComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<PointLightComponent>>().in(boost::di::singleton),
                    boost::di::bind<ComponentStorage<ScriptComponent>>().in(boost::di::singleton),
                    boost::di::bind<WindowWrapper>().to(winWrapper),
                    boost::di::bind<WindowHandle>().to(handle),
                    boost::di::bind<LaunchingParams>().to(params),
                    boost::di::bind<IGraphicsDevice>().in(boost::di::singleton).to<Dx11GraphicsDevice>(),
                    boost::di::bind<IShaderCache>().in(boost::di::singleton).to<DxShaderCache>(),
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
                return commonBindings.create<Engine&>();
            }

            throw std::invalid_argument("Unsupported gpu api");
		}
	};
}