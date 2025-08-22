#pragma once
#include "framework.h"

namespace Potator
{
    struct VertexShaderHandle { size_t Id; };
    struct PixelShaderHandle { size_t Id; };
    struct InputLayoutHandle { size_t Id; };

    class IShaderBinary
    {
    public:
        virtual const void* GetData() const = 0;
		virtual UINT GetSize() const = 0;
        virtual ~IShaderBinary() = default;
    };
}