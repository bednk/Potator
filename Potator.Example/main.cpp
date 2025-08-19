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

using namespace Potator;

int main()
{
	Engine engine;
	auto device = engine.GetGraphicsDevice();
	HlslShader vertexShader(L"D:\\repos\\Potator\\VertexShader_c.cso", ShaderType::Vertex);
	HlslShader pixelShader(L"D:\\repos\\Potator\\PixelShader_c.cso", ShaderType::Pixel);


	VertexBuffer<ColoredVertex> cpuVertexBuffer(
		{
			{{0.0f, 0.5f, 0.0f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f }},
			{{0.5f, -0.5f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f, 1.0f }},
			{{-0.5f, -0.5f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f, 1.0f }}
		}, &vertexShader);
	IndexBuffer cpuIndexBuffer(
		{
			0, 1, 2
		});
	VertexBufferHandle gpuVertexBuffer = device->Create(&cpuVertexBuffer);
	IndexBufferHandle gpuIndexBuffer = device->Create(&cpuIndexBuffer);



	Entity meshEntity = engine.GetEntityRegistry().GetNew();

	MeshComponent mesh = { gpuVertexBuffer, gpuIndexBuffer, 3, 0, 0 };

	TransformComponent meshTransform;

	engine.GetSceneGraph().AddNode(meshEntity, meshTransform);
	engine.GetMeshes().Store(meshEntity, mesh);

	engine.GetTransforms()[meshEntity].Local(2, 3) = 5;

	//std::cout << engine.GetTransforms()[entity].Local.format(Eigen::IOFormat(Eigen::FullPrecision, 0, ", ", "\n", "[", "]")) << "\n";

	device->Bind(&vertexShader);
	device->Bind(&pixelShader);

	Entity camera = engine.GetEntityRegistry().GetNew();
	engine.GetViewManager().Add(camera);
	engine.GetTransforms()[camera].Local(2, 3) = 4.5;


	std::cout << engine.GetTransforms()[meshEntity].Local.format(Eigen::IOFormat(Eigen::FullPrecision, 0, ", ", "\n", "[", "]")) << "\n";

	MovementComponent meshMovement;
	meshMovement.LinearVelocity.z() = 0.05f;

	engine.GetMovements().Store(meshEntity, meshMovement);

	engine.Run();
}