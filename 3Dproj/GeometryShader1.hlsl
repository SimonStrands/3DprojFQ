
struct GSInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos : FRAG_POS;
};
struct GSOutput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float4 fragpos: FRAG_POS;
};

[maxvertexcount(3)]
void main(
	triangle GSInput input[3],
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.position = input[i].position;
		element.uv = input[i].uv;
		element.normal = input[i].normal;
		element.tangent = input[i].tangent;
		element.bitangent = input[i].bitangent;
		element.fragpos = input[i].fragpos;
		output.Append(element);
	}

}