struct VertexShaderInput {
	float3 position : POSITION;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
    
	float4x4 MVP = mul(mul(transform, view),projection);
	output.position = mul((float4((input.position), 1.0f)), MVP);
    
	return output;
}