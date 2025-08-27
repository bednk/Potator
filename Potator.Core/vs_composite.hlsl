#include "structs.hlsli"

cbuffer MeshTransform : register(b0)
{
    matrix World;
};

cbuffer ViewTransform : register(b1)
{
    matrix ViewProj;
};

VsCompositeOut main(VsCompositeIn input)
{
    VsCompositeOut res;
    
    res.PositionOut = mul(input.PositionIn, mul(World, ViewProj));
    res.UvOut = input.UvIn;
    res.ColorOut = input.ColorIn;
    res.Normal = normalize(mul(input.Normal, (float3x3)World));
    res.WorldPos = mul(input.PositionIn, World);
    return res;
}