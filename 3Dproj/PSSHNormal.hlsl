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

cbuffer CBuf
{
	float4 kd;
	float4 ks;
	float4 ka;
};

Texture2D nMap : register(t0); // normal map
Texture2D diffuseTex : register(t1); // diffuse base color
Texture2D ambientTex : register(t2); //normal light(without light)
Texture2D specularTex : register(t3); // specular //shinyness
Texture2D SM : register(t4); // shadow map
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4x4 LVT = lightView[0];
    float4 lightPos = float4(LVT[3][0], LVT[3][1], LVT[3][2], LVT[3][3]);
	float3 posToView = normalize(input.fragpos.xyz - cameraPos.xyz);
	if (dot(posToView, input.normal) > 0) {
		input.normal = -input.normal;
	}
	//do the normal map thing
	float3 nMapNormal;
	float3x3 TBN = float3x3(
		input.tangent.xyz,
		input.bitangent.xyz,
		input.normal.xyz
		);
	const float3 normalSample = nMap.Sample(testSampler, input.uv).xyz;
	nMapNormal.x = normalSample.x * 2.0f - 1.0f;
	nMapNormal.y = normalSample.y * 2.0f - 1.0f;
	nMapNormal.z = normalSample.z * 2.0f - 1.0f;
	input.normal = mul(nMapNormal, (float3x3)TBN);

	if (lightPos.w > 1) {//know this is wrong but eh
		//return float4(input.normal.xyz, 1);
		return float4(input.normal, 0.5f);
	}
	
	//ambient
	//float3 ambient_light = ambientTex.Sample(testSampler,input.uv).xyz * lightColor.xyz;
	float3 ambient_light = ka.xyz * lightColor.xyz;
	float3 diffuseTexKd = diffuseTex.Sample(testSampler, input.uv).xyz;
	float3 specularTexKs = specularTex.Sample(testSampler, input.uv).xyz;
	//float3 ambient_light = float3(0.1,0.1,0.1);

	float3 specular;
	float3 defuse_light;
	//shadow map
	input.shadowMapCoords.xyz = input.shadowMapCoords.xyz / input.shadowMapCoords.w;
	if (SM.Sample(testSampler, input.shadowMapCoords.xy).r > input.shadowMapCoords.z - 0.00001 && 
		input.shadowMapCoords.x < 1 && input.shadowMapCoords.x > 0 &&
		input.shadowMapCoords.y < 1 && input.shadowMapCoords.y > 0){
	
		//defuse
		float3 lightDir = normalize(input.fragpos.xyz - lightPos.xyz);
		float ammount_diffuse = max(dot(-input.normal.xyz, lightDir), 0.0f);
		defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;
		
		//specular
		//float3 ksa = float3(.9, .9, .9);
		float const_spec = 1.0f;
		float3 reflection = normalize(reflect(-lightDir, normalize(input.normal.xyz)));
		float spec = pow(max(0.f, dot(posToView, reflection)), 32);
		specular = ks.xyz * (lightColor.xyz * ammount_diffuse) * const_spec * ks.xyz * spec;
		//specular = ksa * (lightColor.xyz * ammount_diffuse) * const_spec * ksa * spec;
	}
	else {
		defuse_light = specular = float3(0.0f, 0.0f, 0.0f);
	}
	

	//get final lightning
	float3 lightning = saturate(ambient_light + defuse_light) + specular;
	
	//add the texture
	float4 dtex = diffuseTex.Sample(testSampler, input.uv);
	float3 final = (diffuseTex.Sample(testSampler, input.uv).xyz) * lightning;

	return float4(final, dtex.a);

}