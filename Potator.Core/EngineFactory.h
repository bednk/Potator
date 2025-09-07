#pragma once
#include "Engine.h"
#include "Components.h"


namespace Potator
{
	class EngineFactory
	{
    public:
        EngineFactory(const LaunchingParams& params);
        Engine& GetEngine() { return *_engine; }
        Systems& GetSystems() { return *_systems; }
        Components& GetComponents() { return *_components; }
    private:
        std::shared_ptr<Engine> _engine;
        std::shared_ptr<Systems> _systems;
        std::shared_ptr<Components> _components;
	};

}