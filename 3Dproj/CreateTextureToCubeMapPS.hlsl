struct PixelShaderOutput
{
    float4 Color : SV_Target0;
};

PixelShaderOutput main() : SV_TARGET
{
    PixelShaderOutput output;
    output.Color = float4(1, 1, 1, 1);
    return output;
}