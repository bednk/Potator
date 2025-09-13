#include "EngineFactory.h"
#include "CustomExtension.h"

using namespace Potator;

int main()
{
    LaunchingParams params;
    params.Title = "Teapot";
    params.Width = 1280;
    params.Height = 720;

    EngineFactory factory(params);
    auto& engine = factory.GetEngine();
    CustomExtension ext(factory.GetSystems(), factory.GetComponents());
    engine.SetExtension(&ext);
    engine.Run();
}