#include "framework.h"
#include "Engine.h"
#include "EntityRegistry.h"
#include "VertexBuffer.h"
#include <Vertex.h>
#include "HlslShader.h"
#include <string>
#include "ColoredVertex.h"
#include "IndexBuffer.h"

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

	Eigen::Transform<float, 3, Eigen::Affine> t;
	t = Eigen::AngleAxisf(1.57, Eigen::Vector3f::UnitZ());
	Eigen::Matrix4f rot4x4 = t.matrix();

	Buffer<Eigen::Matrix4f> cBuffSource({ rot4x4 });

	BufferHandle gpuVertexBuffer = device->Create(&cpuVertexBuffer);
	BufferHandle gpuIndexBuffer = device->Create(&cpuIndexBuffer);
	BufferHandle gpuCBuffer = device->CreateConstant(&cBuffSource);

	MeshComponent mesh = { gpuVertexBuffer, gpuIndexBuffer, 3, 0, 0 };

	engine.GetMeshes().Store(engine.GetEntityRegistry().GetNew(), mesh);


	

	device->Bind(&vertexShader);
	device->Bind(&pixelShader);
	device->BindConstant(&gpuCBuffer, PipelineStage::VertexShader);
	
	engine.Run();
}