#include "framework.h"
#include "Engine.h"
#include "EntityRegistry.h"
#include "VertexBuffer.h"
#include <Vertex.h>
#include "HlslShader.h"
#include <string>
#include "ColoredVertex.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <iostream>
#include "MovementComponent.h"
#include <MoveCommand.h>
#include <MaterialComponent.h>
#include <Dx11GraphicsDevice.h>
#include <DxShaderCache.h>
#include <WindowWrapper.h>
#include <Systems.h>
#include <boost/di.hpp>


namespace di = boost::di;
using namespace Potator;



int main()
{
    LaunchingParams params;
    WindowWrapper winWrapper = { { sf::VideoMode{ {params.Width, params.Height} }, params.Title } };

    WindowHandle handle = { winWrapper.Window.getNativeHandle() };

    auto injector = di::make_injector(
        di::bind<ComponentStorage<MeshComponent>>().in(di::singleton),
        di::bind<ComponentStorage<MaterialComponent>>().in(di::singleton),
        di::bind<ComponentStorage<TransformComponent>>().in(di::singleton),
        di::bind<ComponentStorage<MovementComponent>>().in(di::singleton),
        di::bind<ComponentStorage<SceneNodeComponent>>().in(di::singleton),
        di::bind<ComponentStorage<CommandQueueComponent>>().in(di::singleton),
        di::bind<ComponentStorage<CameraComponent>>().in(di::singleton),
        di::bind<ComponentStorage<PointLightComponent>>().in(di::singleton),
        di::bind<ComponentStorage<ScriptComponent>>().in(di::singleton),
        di::bind<LaunchingParams>().to(params),
        di::bind<WindowWrapper>().to(winWrapper),
        di::bind<WindowHandle>().to(handle),
        di::bind<IGraphicsDevice>().in(di::singleton).to<Dx11GraphicsDevice>(),
        di::bind<IShaderCache>().in(di::singleton).to<DxShaderCache>(),
        di::bind<SceneGraph>().in(di::singleton),
        di::bind<MeshRenderer>().in(di::singleton),
        di::bind<ViewManager>().in(di::singleton),
        di::bind<MovementSystem>().in(di::singleton),
        di::bind<FixedStepTracker>().in(di::singleton),
        di::bind<CommandDispatcher>().in(di::singleton),
        di::bind<SceneLoader>().in(di::singleton),
        di::bind<Lighting>().in(di::singleton),
        di::bind<WindowHandler>().in(di::singleton),
        di::bind<Systems>().in(di::singleton),
        di::bind<Engine>().in(di::singleton)

    );


    Systems& endadagine = injector.create<Systems&>();
    Engine& engine = injector.create<Engine&>();

    SceneLoader& loader = engine.GetLoader();
    loader.Load("teapot.glb");

    engine.Run();
}