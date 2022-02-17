// Input control point
struct HS_INPUT_FVS
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

// Output patch constant data.
struct HS_CONSTANT_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

cbuffer CBuf
{
    row_major matrix transform; //model
    row_major matrix view;
    row_major matrix projection;
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_OUTPUT CalcHSPatchConstants(
	InputPatch<HS_INPUT_FVS, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_OUTPUT Output;
    float3 camPos = float3(-view[3].x, -view[3].y, -view[3].z);
    float4x4 tv = mul(transform, view);
    float DistToVertex0 = mul(ip[0].position, tv).z;//what wrong here
    float DistToVertex1 = mul(ip[1].position, tv).z;
    float DistToVertex2 = mul(ip[2].position, tv).z;
	//make this dynamic later
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        Output.EdgeTessFactor[i] = 50;
        
    Output.InsideTessFactor = 50;

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
//[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_INPUT_FVS main(
	InputPatch<HS_INPUT_FVS, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    HS_INPUT_FVS Output;
	
    return ip[i];

}
