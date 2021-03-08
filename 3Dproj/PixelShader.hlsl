struct PixelShaderInput 
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
};

cbuffer CBuf
{
	float4 lightPos;
	float4 cameraPos;
	float4 lightColor;
	float4 ka;
	float4 kd;
	float4 ks;
	bool nMapping;
};

Texture2D ambientTex : register(t0);
Texture2D testTex : register(t1);
Texture2D nMap : register(t2);
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 inputnormal = input.normal;
	if (true) {
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
		//return float4(input.normal, 1.0f);
	}

	//ambient
	float3 ambient_light = ka.xyz * lightColor.xyz;

	//defuse
	//for culling none
	float3 posToView = normalize(input.fragpos.xyz - cameraPos.xyz);
	float3 diffuseNormal = inputnormal.xyz;
	if (dot(posToView, diffuseNormal) > 0) {
		diffuseNormal = -diffuseNormal;
	}
	float3 lightDir = normalize(input.fragpos.xyz - lightPos.xyz);
	float ammount_diffuse = max(dot(-diffuseNormal.xyz, lightDir), 0.0f);
	float3 defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;

	//specular
	float const_spec = 2.0f;
	float3 lightToPos = normalize(lightPos.xyz - input.fragpos.xyz);
	float3 reflection = normalize(reflect(lightToPos, normalize(input.normal.xyz)));
	float spec = pow(max(dot(posToView, reflection), 0.f), 32);
	float3 specular = const_spec * spec * ks.xyz * lightColor.xyz;

	//get final lightning
	float3 lightning = (ambient_light + defuse_light) + specular;
	//add the texture
	float3 final = (testTex.Sample(testSampler, input.uv).xyz) * lightning;
	return float4(final, 1.0f);

}