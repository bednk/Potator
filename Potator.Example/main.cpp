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

	Eigen::Matrix4f identity = Eigen::Matrix4f::Identity();

	ConstantBuffer<Eigen::Matrix4f> cBuffSource(identity);

	VertexBufferHandle gpuVertexBuffer = device->Create(&cpuVertexBuffer);
	IndexBufferHandle gpuIndexBuffer = device->Create(&cpuIndexBuffer);
	ConstantBufferHandle gpuCBuffer = device->Create(&cBuffSource);

	MeshComponent mesh = { gpuVertexBuffer, gpuIndexBuffer, 3, 0, 0 };

	engine.GetMeshes().Store(engine.GetEntityRegistry().GetNew(), mesh);

	device->Bind(&vertexShader);
	device->Bind(&pixelShader);
	device->Bind(&gpuCBuffer, PipelineStage::VertexShader, 0);

	Eigen::Transform<float, 3, Eigen::Affine> tb;
	tb = Eigen::AngleAxisf(1.57, Eigen::Vector3f::UnitZ());
	Eigen::Matrix4f rotation = tb.matrix();
	cBuffSource.Update(rotation);
	device->Update(&cBuffSource, &gpuCBuffer);
	
	engine.Run();
}