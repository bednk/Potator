struct PointLight
{
    float4 Color;
    float3 Position;
    float QuadraticAtt;
    float LinearAtt;
    float ConstAtt;
    float2 pad;
};

struct VSOut
{
    float4 PositionOut : SV_Position;
    float4 ColorOut : Color;
    float3 WorldPos : WorldPos;
    float3 Normal : Normal;
    float2 UvOut : Uv;
};

cbuffer Material : register(b0)
{
    float4 MaterialColor;
    int HasTexture;
    int HasColor;
};

cbuffer EnviromentLights : register(b1)
{
    float4 AmbientColor;
    float4 DirectedColor;
    float3 Direction;
    float pad;
};

cbuffer PointLights : register(b2)
{
    PointLight Lights[16];
    uint Count;
    float3 pad2;
}

Texture2D tex;
SamplerState samp;

float4 main(VSOut input) : SV_TARGET
{
    float4 result = input.ColorOut;
    if (HasTexture != 0)
    {
        result = tex.Sample(samp, input.UvOut);
    }
    else if (HasColor != 0)
    {
        result = MaterialColor;
    }
    
    float3 envIntesity = saturate(max(0.0f, dot(input.Normal, -Direction)) * DirectedColor.rgb + AmbientColor.rgb);
    result.rgb *= envIntesity;
    return result;
}