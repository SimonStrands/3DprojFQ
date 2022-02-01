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
    float4 fragPos = gTexPosition.Load(int3(DTid.xy, 0));
    float4 normal = gTexNormal.Load(int3(DTid.xy, 0));
    float4 color = gTexDiffuse.Load(int3(DTid.xy, 0));
					
    if (length(normal.xyz) > 0.2f)//check if there is any object at all
    {			//nroflights
        float4 lightning = float4(0, 0, 0, 0);
        for (int i = 0; i < 2; i++)
        {
            float3 ka = float3(0.1, 0.1, 0.1);
            float3 kd = float3(0.2, 0.2, 0.2);
            float3 ks = float3(0.5, 0.5, 0.5);
            float3 lightColor = float3(1, 1, 1);
            
			//ambient
			//float3 ambient_light = ka.xyz * lightColor.xyz;
            float3 ambient_light = float3(0.1, 0.1, 0.1);
            float3 specular;
            float3 defuse_light;
            
			//calculate if we are in shadow
            const float4 shadowCamera = float4(fragPos.xyz, 1);
            const float4 shadowHomo = mul(shadowCamera, mul(lightView[i], projection));
            float4 shadowMapCoords = shadowHomo * float4(0.5, -0.5, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);
			shadowMapCoords.xyz = shadowMapCoords.xyz / shadowMapCoords.w;
            float4 SM = shadowMapping.Load(int4(DTid.xy, i, 0));
			if (SM.r > shadowMapCoords.z - 0.0000001 &&
				shadowMapCoords.x < 1 && shadowMapCoords.x > 0 &&
				shadowMapCoords.y < 1 && shadowMapCoords.y > 0)
            {
                float4x4 LVT = lightView[i];
                float4 lightPos = float4(LVT[3][0], LVT[3][1], LVT[3][2], LVT[3][3]);
                
                float3 posToView = normalize(fragPos.xyz - cameraPos.xyz);
				//////calc lightning//////
				//defuse
                float3 lightDir = normalize(fragPos.xyz - lightPos.xyz);
                float ammount_diffuse = max(dot(-normal.xyz, lightDir), 0.0f);
                defuse_light = ammount_diffuse * kd.xyz * lightColor.xyz;
                
                //specular
                float const_spec = 1.0f;
                float3 reflection = normalize(reflect(-lightDir, normalize(normal.xyz)));
                float spec = pow(max(0.f, dot(posToView, reflection)), 32);
                specular = ks.xyz * (lightColor.xyz * ammount_diffuse) * const_spec * ks.xyz * spec;
                
                		//get final lightning
                float3 lightning = saturate(ambient_light + defuse_light) + specular;

		        //add the texture
                
            }
            else
            {
				//we are in shadow
                lightning += float4(ambient_light, 0);
            }
        }
        float4 final = color * lightning;
        backBuffer[DTid.xy] = float4(final.xyz, 0.9);
    }	
}

//1920*1080
