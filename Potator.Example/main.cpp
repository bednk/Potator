#include "framework.h"
#include "Engine.h"
#include "EntityRegistry.h"
#include "VertexBuffer.h"
#include <Vertex.h>
#include "HlslShader.h"
#include <string>

using namespace Potator;

int main()
{
	Engine engine;
	auto device = engine.GetGraphicsDevice();
	HlslShader vertexShader(L"D:\\repos\\Potator\\VertexShader.cso", ShaderType::Vertex);
	HlslShader pixelShader(L"D:\\repos\\Potator\\PixelShader.cso", ShaderType::Pixel);
	VertexBuffer<Vertex> vertexBuffer({ {0.0f, 0.5f, 0.0f }, {0.5f, -0.5f, 0.0f }, {-0.5f, -0.5f, 0.0f } }, &vertexShader);

	device->Bind(&vertexBuffer);
	device->Bind(&vertexShader);
	device->Bind(&pixelShader);
	
	engine.Run();
}