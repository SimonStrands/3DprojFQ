//git
struct VertexShaderInput {
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
	float4 shadowMapCoords : SM_COORDS;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
	row_major matrix lightView;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	//input.position.z = 0.1 * input.position.z;//does this work?

	float4x4 SHMVP = mul(mul(transform, lightView), projection);
	float4 shadowCamera = mul(float4(input.position, 1.0f), transform);
	float4 shadowHomo = mul(shadowCamera, SHMVP);
	output.shadowMapCoords = shadowHomo * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w;

	float4x4 MVP = mul(mul(transform, view),projection);
	output.fragpos = mul(float4(input.position,1.0f), transform);
	output.position = mul((float4((input.position), 1.0f)), MVP);
	output.uv = input.uv;
	output.bitangent = normalize((mul(input.bitangent, transform)).xyz);
	output.tangent = normalize((mul(input.tangent, transform)).xyz);
	output.normal = normalize((mul(input.normal, transform)).xyz);
	//output.position.z = 0.1 * output.position.z;//does this work?
	

	return output;
}