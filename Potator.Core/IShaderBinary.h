#pragma once

namespace Potator
{
    enum class ShaderType { Vertex, Pixel };

    class IShaderBinary
    {
    public:
        virtual const void* GetData() const = 0;
		virtual UINT GetSize() const = 0;
        virtual ShaderType GetType() const = 0;
        virtual ~IShaderBinary() = default;
    };
}