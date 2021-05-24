struct VertexShaderInput {
	float4 position : POSITION;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float4 fragpos: FRAG_POS;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	float4x4 MVP = mul(mul(transform, view),projection);
	output.fragpos = mul(float4(input.position,1.0f), transform);
	output.position = mul((float4((input.position), 1.0f)), MVP);
	return output;
}