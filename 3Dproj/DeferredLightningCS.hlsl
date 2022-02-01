#include "Transforms.hlsli"
RWTexture2D<unorm float4> backBuffer;
//another gbuffer for shadowMapCoords?
Texture2D<float4> gTexPosition : register(t0);
Texture2D<float4> gTexNormal : register(t1);
Texture2D<float4> gTexDiffuse : register(t2);
Texture2DArray<float4> shadowMapping : register(t3);
//SamplerState testSampler;

[numthreads(32, 8, 1)]//32 16
void main( uint3 DTid : SV_DispatchThreadID )
{
	//backBuffer[DTid.xy] = float4(255, 0, 0, 1);
	
	//backBuffer[DTid.xy] = float4(shadowMapping[DTid.xy].x 0, 0, 1.0);
	//backBuffer[DTid.xy] = float4(shadowMapping.Sample(testSampler, float3(1,1, 0)).xyz ,1);
	
    //float4 returns = float4(gTexDiffuse.Load(int3(DTid.xy, 0)));
    //backBuffer[DTid.xy] = returns;

    float4 returns = float4(0, 0, 0, 0);
   // float3 a = shadowMapping.Load(int3(DTid.xy, 1));
    returns = shadowMapping.Load(int4(DTid.xy, 1, 0));
    //returns += shadowMapping.Load(int4(DTid.xy, 0, 0));
	//float depth = shadowMapping.Load(int3(DTid.xy, 0));
    
    if (returns.x >= 1)
    {
        backBuffer[DTid.xy] = float4(1, 0, 0, 1);
    }
    else if(returns.x < 1 && returns.x > 0.5)
    {
        backBuffer[DTid.xy] = float4(0, 0, 0, 1);
    }
    else{
        backBuffer[DTid.xy] = float4(0, 1, 0, 1);
    }
	
	
	
	//backBuffer[DTid.xy] = float4(DTid.xy / float2(100,100), 0, 1);

	//float3 kd = float3(0.1, 0.1, 0.1);
	/*
	float3 kd = float3(0.2, 0.2, 0.2);
	float3 ks = float3(0.5, 0.5, 0.5);
	float3 lightColor = float3(1, 1, 1);
	float4 fragpos = float4(gTexPosition[DTid.xy].xyz, 1.0);
	float4 Normal = float4(gTexNormal[DTid.xy].xyz, 1.0);
	float4 dtex = float4(gTexDiffuse[DTid.xy].xyz, 1.0);
	
	if (length(Normal.xyz) > 0.2f) {
		float3 posToView = normalize(fragpos.xyz - cameraPos.xyz);

		//ambient
		//float3 ambient_light = ka.xyz * lightColor.xyz;
		float3 ambient_light = float3(0.1, 0.1, 0.1);

		float3 specular;
		float3 defuse_light;
		//input.shadowMapCoords.xyz = input.shadowMapCoords.xyz / input.shadowMapCoords.w;
		//if (SM.Sample(testSampler, input.shadowMapCoords.xy).r > input.shadowMapCoords.z - 0.00001 &&
		//	input.shadowMapCoords.x < 1 && input.shadowMapCoords.x > 0 &&
		//	input.shadowMapCoords.y < 1 && input.shadowMapCoords.y > 0) {

		//defuse
		float3 lightDir = normalize(fragpos.xyz - lightPos.xyz);
		float ammount_diffuse = max(dot(-Normal.xyz, lightDir), 0.0f);
		defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;

		//specular
		float const_spec = 1.0f;
		float3 reflection = normalize(reflect(-lightDir, normalize(Normal.xyz)));
		float spec = pow(max(0.f, dot(posToView, reflection)), 32);
		specular = ks.xyz * (lightColor.xyz * ammount_diffuse) * const_spec * ks.xyz * spec;
		//}
		//else {
		//	defuse_light = specular = float3(0.0f, 0.0f, 0.0f);
		//}


		//get final lightning
		float3 lightning = saturate(ambient_light + defuse_light) + specular;


		//add the texture
		//add the texture
		float3 final = dtex.xyz * lightning.xyz;

		//return float4(final, dtex.a);
		backBuffer[DTid.xy] = float4(final, 0.9);
		//backBuffer[DTid.xy] = float4(0,0,0,0.9);
		//backBuffer[DTid.xy] = float4(dtex.xyz, 1.0);
	}
	*/
}

//1920*1080
