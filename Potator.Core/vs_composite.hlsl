struct VSIn
{
    float4 PositionIn : Position;
    float2 UvIn : Uv;
    float4 ColorIn : Color;
};

struct VSOut
{
    float2 UvOut : Uv;
    float4 ColorOut : Color;
    float4 PositionOut : SV_Position;
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
    return res;
}