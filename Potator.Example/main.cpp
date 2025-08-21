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
	auto device = engine.GetGraphicsDevice();


	HlslShader vsBinary(L"D:\\repos\\Potator\\VertexShader_c.cso");
	HlslShader psBinary(L"D:\\repos\\Potator\\PixelShader_c.cso");

	VertexBuffer<ColoredVertex> cpuVertexBuffer(
		{
			{{0.0f, 0.5f, 0.0f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
			{{0.5f, -0.5f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f, 1.0f }},
			{{-0.5f, -0.5f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f, 1.0f }}
		});
	IndexBuffer cpuIndexBuffer(
		{
			0, 1, 2
		});

	MaterialComponent material =
	{
		device->CreateVertexShader(&vsBinary),
		&vsBinary,
		device->CreatePixelShader(&psBinary),
		device->CreateInputLayout(cpuVertexBuffer.GetVertexLayout(), &vsBinary)
	};

	VertexBufferHandle gpuVertexBuffer = device->Create(&cpuVertexBuffer);
	IndexBufferHandle gpuIndexBuffer = device->Create(&cpuIndexBuffer);
	Entity meshEntity = engine.GetEntityRegistry().GetNew();
	MeshComponent mesh = { gpuVertexBuffer, gpuIndexBuffer, 3, 0, 0 };
	TransformComponent meshTransform;
	engine.GetSceneGraph().AddNode(meshEntity, meshTransform);
	engine.GetMeshes().Store(meshEntity, mesh);
	engine.GetMaterials().Store(meshEntity, material);
	engine.GetTransforms()[meshEntity].Local(2, 3) = 5;

	engine.Run();
}