struct PointLight
{
    float4 Color;
    float3 Position;
    float QuadraticAtt;
    float LinearAtt;
    float ConstAtt;
    float2 _pad;
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
    float _pad;
    uint PointLightsCount;
};

Texture2D Tex : register(t0);
StructuredBuffer<PointLight> Lights : register(t1);
SamplerState Samp;

float4 main(VSOut input) : SV_TARGET
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
    
    
    
    float3 envIntensity = saturate(max(0.0f, dot(normalize(input.Normal), -Direction)) * DirectedColor.rgb + AmbientColor.rgb);
    
    float3 litColor = result.rgb * envIntensity;

    for (uint i = 0; i < min(PointLightsCount, 16); ++i)
    {
        float3 toLight = Lights[0].Position - input.WorldPos;
        float distance = length(toLight);
        float3 directionToLight = toLight / distance;

        float diffuse = max(0.0f, dot(normalize(input.Normal), directionToLight));

        float luminosity = 1.0f / (Lights[0].ConstAtt +
                                Lights[0].LinearAtt * distance +
                                Lights[0].QuadraticAtt * distance * distance);

        litColor += result.rgb * Lights[0].Color.rgb * diffuse * luminosity;
    }
    
    result.rgb = litColor;
    return result;
}