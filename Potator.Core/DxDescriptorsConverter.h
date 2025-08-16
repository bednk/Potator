#pragma once
#include <d3d11.h>
#include "VertexMemberDescriptor.h"
#include "Exception.h"

namespace Potator
{
	class DxDescriptorsConverter
	{
	public:
		static D3D11_INPUT_ELEMENT_DESC GetInputElementDesc(const VertexMemberDescriptor& descriptor)
		{
			D3D11_INPUT_ELEMENT_DESC result = {};
			result.SemanticName = descriptor.Name.c_str();
			result.AlignedByteOffset = descriptor.Offset;
			result.Format = GetDxGiFormat(descriptor.Format);

			result.SemanticIndex = 0;
			result.InputSlot = 0;
			result.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			result.InstanceDataStepRate = 0;
			return result;
		}

		static DXGI_FORMAT GetDxGiFormat(const DataFormat& format)
		{
			switch (format)
			{
			case DataFormat::float4:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			default:
				throw Exception("Unsupported DXGI_FORMAT");
			}
		}
	};
}