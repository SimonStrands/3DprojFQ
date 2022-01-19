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

	float4x4 MVP = mul(mul(transform, view), projection);
	output.fragpos = mul(float4(input.position, 1.0f), transform);
	output.position = mul((float4((input.position), 1.0f)), MVP);
	output.uv = input.uv;
	output.bitangent = normalize((mul(float4(input.bitangent,0.0f), transform)).xyz);
	output.tangent = normalize((mul(float4(input.tangent,0.f), transform)).xyz);
	output.normal = normalize((mul(float4(input.normal,0.f), transform)).xyz);


	const float4 shadowCamera = mul(float4(input.position, 1.0f), transform);
	const float4 shadowHomo = mul(shadowCamera, mul(lightView, projection));
	output.shadowMapCoords = shadowHomo * float4(0.5, -0.5, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);

	return output;
}