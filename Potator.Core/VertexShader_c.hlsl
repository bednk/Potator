struct Result
{
    float4 Col : Color;
    float4 Pos : SV_Position;
};

cbuffer MeshTransform : register(b0)
{
    matrix World;
};

Result main(float4 pos : Position, float4 col : Color)
{
    Result res;
    res.Pos = mul(pos, World);
    res.Col = col;
    return res;
}