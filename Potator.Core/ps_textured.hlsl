Texture2D tex; //or directly : register(t0)
SamplerState samp;

float4 main(float2 uv : Uv) : SV_TARGET
{
    float4 result = tex.Sample(samp, uv);
    return result;
}