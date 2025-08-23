struct Result
{
    float2 Tex : Uv;
    float4 Pos : SV_Position;
};

cbuffer MeshTransform : register(b0)
{
    matrix World;
};

cbuffer ViewTransform : register(b1)
{
    matrix ViewProj;
};

Result main(float4 pos : Position, float2 tex : Uv)
{
    Result res;
    res.Pos = mul(pos, mul(World, ViewProj));
    res.Tex = tex;
    return res;
}