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

using namespace Potator;

int main()
{
	Engine engine;

	SceneLoader& loadear = engine.GetLoader();
	loadear.Load("chicken.glb");

	engine.Run();
}