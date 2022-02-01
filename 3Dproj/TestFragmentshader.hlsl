#include "Transforms.hlsli"
struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
	float4 shadowMapCoords : SM_COORDS;
};

struct PixelShaderOutput {
	float4 Position : SV_Target0;//depth
	float4 Normal :	SV_Target1;
	float4 Color :	SV_Target2;
};

cbuffer CBuf
{
	float4 kd;
	float4 ks;
	float4 ka;
};

Texture2D diffuseTex : register(t1); // diffuse base color
Texture2D ambientTex : register(t2); //normal light(without light)
Texture2D specularTex : register(t3); // specular //shinyness
Texture2D SM : register(t4); // shadow map
SamplerState testSampler;


//float4 main(PixelShaderInput input) : SV_TARGET
PixelShaderOutput main(PixelShaderInput input)
{
	PixelShaderOutput output;
	
	//calc normal
	float3 posToView = normalize(input.fragpos.xyz - cameraPos.xyz);
	if (dot(posToView, input.normal) > 0) {
		input.normal = -input.normal;
	}
	////else here calc normalMapping

	output.Normal = float4(input.normal.xyz, 1);
	output.Position = float4(input.fragpos);
	output.Color = diffuseTex.Sample(testSampler, input.uv);
	//TEST
	//output.Position = float4(1, 0, 0, 1);
	//output.Normal =   float4(0, 1, 0, 1);
	//output.Color =    float4(0, 0, 1, 1);
	
	return output;
	//return float4(1,0,0,1);

}
