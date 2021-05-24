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
};

Texture2D ambientTex : register(t0);
Texture2D testTex : register(t1);
Texture2D nMap : register(t2);
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
	float x = input.fragpos.x - cameraPos.x;
	float y = input.fragpos.y - cameraPos.y;
	float z = input.fragpos.z - cameraPos.z;
	float d = 1/(sqrt(x * x + y * y + z * z)/3);
	return float4(d, d, d, 1);
}

/*float4 main(PixelShaderInput input) : SV_TARGET
{
	//for culling none
	float3 posToView = normalize(input.fragpos.xyz - cameraPos.xyz);
	if (dot(posToView, input.normal) > 0) {
		input.normal = -input.normal;
	}
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
		
	}
	
	//ambient
	float3 ambient_light = ka.xyz * lightColor.xyz;

	//defuse
	float3 lightDir = normalize(input.fragpos.xyz - lightPos.xyz);
	float ammount_diffuse = max(dot(-input.normal.xyz, lightDir), 0.0f);
	float3 defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;

	//specular
	float const_spec = 1.0f;
	float3 reflection = normalize(reflect(-lightDir, normalize(input.normal.xyz)));
	float spec = pow(max(0.f, dot(posToView, reflection)), 32);
	float3 specular = ks.xyz * (lightColor.xyz * ammount_diffuse) * const_spec * ks.xyz * spec;

	

	//get final lightning
	float3 lightning = saturate(ambient_light + defuse_light);
	//float3 lightning = (ambient_light + defuse_light) + specular;
	//add the texture
	float4 dtex = testTex.Sample(testSampler, input.uv);
	float3 final = (dtex.xyz * lightning) + specular;
	//float3 final = (testTex.Sample(testSampler, input.uv).xyz) * lightning;
	return float4(final, dtex.a);

}*/