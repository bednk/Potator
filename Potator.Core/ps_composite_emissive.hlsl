#include "structs.hlsli"

cbuffer Material : register(b0)
{
    float4 MaterialColor;
    float SpecularExponent;
    float SpecularIntensity;
    int HasTexture;
    int HasColor;
};

cbuffer Camera : register(b2)
{
    float4 CameraWorldPos;
}

float4 main(VsCompositeOut input) : SV_TARGET
{
    float3 normal = normalize(input.Normal);
    float3 toCamera = normalize(CameraWorldPos.xyz - input.WorldPos);

    float intensity = saturate(dot(normal, toCamera));
    
    float3 cold = MaterialColor.rgb * 0.4;
    float3 base = MaterialColor.rgb;
    float3 hot = lerp(base, float3(1.0, 0.9, 0.6), 0.6);
    
    float3 color = lerp(cold, base, intensity);
    color = lerp(color, hot, pow(intensity, 3));

    return float4(color, 1.0f);
}