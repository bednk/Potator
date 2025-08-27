struct PointLight
{
    float4 Color;
    float3 Position;
    float QuadraticAtt;
    float LinearAtt;
    float ConstAtt;
    float2 _pad;
};

struct VsCompositeOut
{
    float4 PositionOut : SV_Position;
    float4 ColorOut : Color;
    float3 WorldPos : WorldPos;
    float3 Normal : Normal;
    float2 UvOut : Uv;
};

struct VsCompositeIn
{
    float4 PositionIn : Position;
    float4 ColorIn : Color;
    float3 Normal : Normal;
    float2 UvIn : Uv;
};