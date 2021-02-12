struct PixelShaderInput 
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 fragpos: FRAG_POS;
};

//must change cbuf to pixelShader buf
cbuffer CBuf
{
	float4 lightPos;
	float4 cameraPos;
	float4 lightColor;
	float4 ka;
	float4 kd;
	float4 ks;
};

Texture2D testTex : register(t0);
Texture2D nMap : register(t1);
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
	//const float3 normalSample = nMap.Sample(testSampler, input.uv).xyz;
	//input.normal.x = normalSample.x * 2.0f - 1.0f;
	//input.normal.y = -normalSample.y * 2.0f + 1.0f;
	//input.normal.z = -normalSample.z;

	//ambient
	float3 ambient_light = ka.xyz * lightColor.xyz;

	//defuse
	float3 lightDir = normalize(input.fragpos.xyz - lightPos.xyz);
	float ammount_diffuse = max(dot(input.normal.xyz, lightDir), 0.0f);
	float3 defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;

	//specular
	float const_spec = 2.0f;
	float3 lightToPos = normalize(lightPos.xyz - input.fragpos.xyz);
	float3 reflection = normalize(reflect(lightToPos, normalize(input.normal.xyz)));
	float3 posToView = normalize(input.fragpos.xyz - cameraPos.xyz);
	float spec = pow(max(dot(posToView, reflection), 0.f), 32);
	float3 specular = const_spec * spec * ks.xyz * lightColor.xyz;
	//get final lightning
	float3 lightning = (ambient_light + defuse_light) + specular;
	//add the texture
	float3 final = (testTex.Sample(testSampler, input.uv).xyz) * lightning;
	return float4(final, 1.0f);

}