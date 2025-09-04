#pragma once
#include "MeshRenderer.h"
#include "SceneGraph.h"
#include "ViewManager.h"
#include "MovementSystem.h"
#include "FixedStepTracker.h"
#include "CommandDispatcher.h"
#include "Lighting.h"
#include "SceneLoader.h"
#include "WindowHandler.h"
#include "ScriptingSystem.h"

namespace Potator
{
	struct Systems
	{
		Systems(MeshRenderer& renderer,
			SceneGraph& sceneGraph,
			ViewManager& views,
			MovementSystem& movementSystem,
			FixedStepTracker& fixedStepTracker,
			CommandDispatcher& commandDispatcher,
			Lighting& lighting,
			SceneLoader& loader,
			WindowHandler& windowHandler,
			ScriptingSystem& scripting) : 
				Renderer{ renderer },
				SceneGraph{sceneGraph},
				Views{views},
				MovementSystem{movementSystem},
				FixedStepTracker{ fixedStepTracker },
				CommandDispatcher{commandDispatcher},
				Lighting{lighting},
				Loader{loader},
				WindowHandler{windowHandler},
				Scripting{scripting}
		{

		}
		MeshRenderer& Renderer;
		SceneGraph& SceneGraph;
		ViewManager& Views;
		MovementSystem& MovementSystem;
		FixedStepTracker& FixedStepTracker;
		CommandDispatcher& CommandDispatcher;
		Lighting& Lighting;
		SceneLoader& Loader;
		WindowHandler& WindowHandler;
		ScriptingSystem& Scripting;
	};
}