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


	
	Entity entity = engine.GetEntityRegistry().GetNew();

	MeshComponent mesh = { gpuVertexBuffer, gpuIndexBuffer, 3, 0, 0 };

	//Eigen::Transform<float, 3, Eigen::Affine> tb;
	//tb = Eigen::AngleAxisf(1.57, Eigen::Vector3f::UnitZ());
	//Eigen::Matrix4f rotation = tb.matrix();
	TransformComponent transform;

	engine.GetSceneGraph().AddNode(entity, transform);
	engine.GetMeshes().Store(entity, mesh);

	engine.GetTransforms()[entity].Local(3, 2) = 5;

	std::cout << engine.GetTransforms()[entity].Local.format(Eigen::IOFormat(Eigen::FullPrecision, 0, ", ", "\n", "[", "]")) << "\n";

	device->Bind(&vertexShader);
	device->Bind(&pixelShader);

	Entity camera = engine.GetEntityRegistry().GetNew();
	engine.GetViewManager().Add(camera);

	//engine.GetTransforms()[camera].Local(2, 3) = 0;
	//engine.GetTransforms()[camera].Local(0, 3) = 0.5;

	//Eigen::Transform<float, 3, Eigen::Affine> tb;
	//tb = Eigen::AngleAxisf(0.1, Eigen::Vector3f::UnitY());
	//Eigen::Matrix4f rotation = tb.matrix();
	
	std::cout << engine.GetTransforms()[entity].Local.format(Eigen::IOFormat(Eigen::FullPrecision, 0, ", ", "\n", "[", "]")) << "\n";
	//engine.GetTransforms()[camera].Local = rotation;

	
	engine.Run();
}