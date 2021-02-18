//git
struct VertexShaderInput {
	float3 position : POSITION;
	float2 uv : UV;
	float4 normal : NORMAL;
	//float4 tangent : TANGENT;
	//float4 bitangent : BITANGENT;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	
	float4x4 MVP = mul(mul(transform, view),projection);
	output.fragpos = mul(float4(input.position,1.0f), transform);
	output.position = mul((float4((input.position), 1.0f)), MVP);
	output.uv = input.uv;

	output.normal = normalize(mul(input.normal, transform));
	//output.tangent = normalize((mul(float4(input.tangent,0.0f), transform)).xyz);
	//output.bitangent = normalize((mul(float4(input.bitangent, 0.0f), transform)).xyz);

	
	return output;
}