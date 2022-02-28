//another gbuffer for shadowMapCoords?
//maps
cbuffer Time
{
    float dt;
};
RWBuffer<float3> particlePos;
[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    particlePos[DTid.x] = float3(0, 0, 0);
}
