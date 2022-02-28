//another gbuffer for shadowMapCoords?
//maps
cbuffer Time
{
    float dt;
};
RWBuffer<float3> particlePos;
[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //particlePos[DTid.x][0] = cos(0);
    //particlePos[DTid.x][1] = sin(0);
    //particlePos[DTid.x][2] = tan(0);

}
