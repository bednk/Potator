cbuffer LightsGlobalSettings : register(b1)
{
    float4 AmbientColor;
    float4 DirectedColor;
    float3 Direction;
    float _pad;
    uint PointLightsCount;
};