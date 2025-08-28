#include "structs.hlsli"

cbuffer Material : register(b0)
{
    float4 MaterialColor;
    float SpecularExponent;
    float SpecularIntensity;
    int HasTexture;
    int HasColor;
};

cbuffer LightsGlobalSettings : register(b1)
{
    float4 AmbientColor;
    float4 DirectedColor;
    float3 Direction;
    float _pad;
    uint PointLightsCount;
};

cbuffer Camera : register(b2)
{
    float4 CameraWorldPos;
}

Texture2D Tex : register(t0);
StructuredBuffer<PointLight> Lights : register(t1);
SamplerState Samp;

float4 main(VsCompositeOut input) : SV_TARGET
{
    float4 result = input.ColorOut;
    if (HasTexture != 0)
    {
        result = Tex.Sample(Samp, input.UvOut);
    }
    else if (HasColor != 0)
    {
        result = MaterialColor;
    }
    
    float3 ambient = result.rgb * saturate(max(0.0f, dot(normalize(input.Normal), -Direction)) * DirectedColor.rgb + AmbientColor.rgb);
    
    float3 litColor = float3(0.0f, 0.0f, 0.0f);
    
    float3 toCamera = normalize(CameraWorldPos.xyz - input.WorldPos);
    [unroll]
    for (uint i = 0; i < min(PointLightsCount, 16); ++i)
    {
        float3 toLight = Lights[i].Position - input.WorldPos;
        float3 reflectedToLight = normalize(-1 * reflect(toLight, input.Normal));
        
        float distanceToLight = length(toLight);
        float3 directionToLight = toLight / distanceToLight;

        float pointDiffuse = max(0.0f, dot(normalize(input.Normal), directionToLight));

        float luminosity = 1.0f / (Lights[i].ConstAtt +
                                Lights[i].LinearAtt * distanceToLight +
                                Lights[i].QuadraticAtt * distanceToLight * distanceToLight);
        
        float3 baseDifuseColor = result.rgb * Lights[i].Color.rgb;
        float3 diffuse = baseDifuseColor * pointDiffuse * luminosity;
        
        float specularFactor = pow(max(0, dot(toCamera, reflectedToLight)), SpecularExponent) * SpecularIntensity;
        float3 specular = luminosity * result.rgb * Lights[i].Color.rgb * specularFactor;
        
        litColor += diffuse + specular;
    }
    
    result.rgb = saturate(litColor + ambient);
    return result;
}