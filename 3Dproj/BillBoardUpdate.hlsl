RWTexture2D<unorm float4> backBuffer;
//another gbuffer for shadowMapCoords?
//maps

[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //float4 fragPos = gTexPosition.Load(int3(DTid.xy, 0));

}
