#include "pch.h"
#include "HlslShader.h"
#include "D3dcompiler.h"
#include "HrCheck.h"

Potator::HlslShader::HlslShader(std::wstring name, ShaderType type) : _type { type }
{
	D3DReadFileToBlob(name.c_str(), &_blob) >> HrCheck::Instance();
}

const void* Potator::HlslShader::GetData() const
{
	return _blob->GetBufferPointer();
}

UINT Potator::HlslShader::GetSize() const
{
	return _blob->GetBufferSize();
}

Potator::ShaderType Potator::HlslShader::GetType() const
{
	return _type;
}
