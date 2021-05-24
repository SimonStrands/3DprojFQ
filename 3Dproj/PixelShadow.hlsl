//struct ShadowPixelShader(PixelInputType input) : SV_TARGET
//{
//   float4 position : SV_POSITION;
//    float4 fragpos: FRAG_POS;
//};
//
//cbuffer CBuf
//{
//    float4 lightPos;
//};
//
//bias = 0.001f;
//
//color = ambientColor;
//
//projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
//projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
//
//if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y)){
//
//    depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;
//
//    lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
//
//    lightDepthValue = lightDepthValue - bias;
//
//    if (lightDepthValue < depthValue){
//        lightIntensity = saturate(dot(input.normal, input.lightPos));
//
//        if (lightIntensity > 0.0f){
//            color += (diffuseColor * lightIntensity);
//            color = saturate(color);
//        }
//    }
//}
//
//textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);
//
//color = color * textureColor;
//
//return color;
//}

float4 main() : SV_TARGET
{
	return float4(0,0,0,1);
}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//	return float4(0,0,0,1);
//}
