Texture2D tex;
SamplerState samp;

float4 main(float2 uv : Uv) : SV_TARGET
{
    float4 result = tex.Sample(samp, uv);
    return result;
}