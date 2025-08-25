cbuffer Material : register(b0)
{
    float4 MaterialColor;
    int HasTexture;
    int HasColor;
};

struct VSOut
{
    float2 UvOut : Uv;
    float4 ColorOut : Color;
    float4 PositionOut : SV_Position;
};

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
    
    return result;
}