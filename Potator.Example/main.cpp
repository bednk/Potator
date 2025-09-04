#include "framework.h"
#include "EngineFactory.h"

using namespace Potator;

int main()
{
    LaunchingParams params;

    auto& engine = EngineFactory::GetEngine(params);
    SceneLoader& loader = engine.GetLoader();
    loader.Load("teapot.glb");

    engine.Run();
}