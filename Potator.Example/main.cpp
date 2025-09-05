#include "EngineFactory.h"
#include "CustomExtension.h"

using namespace Potator;

int main()
{
    LaunchingParams params;

    EngineFactory factory(params);
    auto& engine = factory.GetEngine();
    CustomExtension ext(factory.GetSystems(), factory.GetComponents());
    engine.SetExtension(&ext);
    engine.Run();
}