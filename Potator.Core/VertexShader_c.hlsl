struct Result
{
    float4 Col : Color;
    float4 Pos : SV_Position;
};

cbuffer MeshTransform : register(b0)
{
    matrix World;
};

cbuffer ViewTransform : register(b1)
{
    matrix Camera;
};

Result main(float4 pos : Position, float4 col : Color)
{
    Result res;
    res.Pos = mul(pos, mul(World, Camera));
    res.Col = col;
    return res;
}