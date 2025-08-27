struct VSIn
{
    float4 PositionIn : Position;
    float4 ColorIn : Color;
    float3 Normal : Normal;
    float2 UvIn : Uv;
};

struct VSOut
{
    float4 PositionOut : SV_Position;
    float4 ColorOut : Color;
    float3 WorldPos : WorldPos;
    float3 Normal : Normal;
    float2 UvOut : Uv;
};

cbuffer MeshTransform : register(b0)
{
    matrix World;
};

cbuffer ViewTransform : register(b1)
{
    matrix ViewProj;
};

VSOut main(VSIn input)
{
    VSOut res;
    
    res.PositionOut = mul(input.PositionIn, mul(World, ViewProj));
    res.UvOut = input.UvIn;
    res.ColorOut = input.ColorIn;
    res.Normal = normalize(mul(input.Normal, (float3x3)World));
    res.WorldPos = mul(input.PositionIn, World);
    return res;
}