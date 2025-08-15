float4 main( float4 pos : POSITION ) : SV_POSITION
{
    return float4(pos.x, pos.y, 0.0f, 1.0f);
}