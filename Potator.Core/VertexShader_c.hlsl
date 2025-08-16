struct Result
{
    float4 Col : Color;
    float4 Pos : SV_Position;
};

Result main(float4 pos : Position, float4 col : Color)
{
    Result res;
    res.Pos = pos;
    res.Col = col;
    return res;
}