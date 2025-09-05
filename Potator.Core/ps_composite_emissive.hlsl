#include "structs.hlsli"
#include "ps_material_cbuff.hlsli"
#include "ps_camera_cbuff.hlsli"


float4 main(VsCompositeOut input) : SV_TARGET
{
    float3 normal = normalize(input.Normal);
    float3 toCamera = normalize(CameraWorldPos.xyz - input.WorldPos);

    float intensity = saturate(dot(normal, toCamera));
    
    float3 cold = MaterialColor.rgb * 0.8;
    float3 hot = MaterialColor.rgb + float3(0.2, 0.2, 0.2);
    
    float3 color = lerp(cold, hot, pow(intensity, 2));

    return float4(color, 1.0f);
}