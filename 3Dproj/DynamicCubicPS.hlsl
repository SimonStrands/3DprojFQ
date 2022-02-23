
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 fragpos : FRAG_POS;
};

struct PixelShaderOutput
{
    float4 Position : SV_Target0; //depth
    float4 Normal : SV_Target1; //normal
    float4 Color : SV_Target2; //map_kd * kd
    float4 ambient : SV_Target3; //map_ka * ka
    float4 specular : SV_Target4; //map_ks * ks //can I put NS as last element?
};
cbuffer CBuf
{
    float4 kd;
    float4 ks; //last element is ns
    float4 ka;
};

TextureCube mirrorTex : register(t0);
SamplerState testSampler;

PixelShaderOutput main(PixelShaderInput input) : SV_TARGET
{
    PixelShaderOutput output;
    //float3 posToFrag = input.fragpos;
    output.Normal = float4(input.normal.xyz, 1);
    output.Position = float4(input.fragpos);
    //output.Color = mirrorTex.Sample(testSampler, output.Position.xyz);
    //output.Color = mirrorTex.Sample(testSampler, float3());
    output.Color = float4(0, 0, 1, 1);
    //output.Color = float4(1, 0, 0, 1);
    //output.ambient =    ambientTex.Sample(testSampler, input.uv) * ka;
    output.ambient = float4(0.5, 0.5, 0.5, 1);
    //output.specular =   specularTex.Sample(testSampler, input.uv) * ks;
    output.specular = float4(0.8, 0.8, 0.8, 0) * ks;
    output.specular.w = ks.w;
    
    return output;
    
}