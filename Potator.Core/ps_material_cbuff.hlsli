cbuffer Material : register(b0)
{
    float4 MaterialColor;
    float SpecularExponent;
    float SpecularIntensity;
    int HasTexture;
    int HasColor;
};