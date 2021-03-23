struct VertexShaderInput {
	float3 position : POSITION;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	row_major float4x4 modelView : MV;
	row_major float4x4 model : MODEL;
	row_major float4x4 modelViewProj: MVP;
};

cbuffer CBuf
{
	row_major matrix transform;//model
	row_major matrix view;
	row_major matrix projection;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;

	float4x4 MVP = mul(mul(transform, view), projection);
	//output.position = mul(float4(input.position, 1.0f), transform);
	//output.fragpos = mul(float4(input.position, 1.0f), MVP);

	output.position = float4(input.position, 1.f);
	output.model = transform;
	output.modelView = mul(transform, view);
	output.modelViewProj = MVP;

	return output;
}