

struct GSInput
{
	float4 position : SV_POSITION;
	float4 fragpos: FRAG_POS;
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

cbuffer CBuf
{
	float4 cameraPos;
	float2 uv;
	float2 padding;
};

[maxvertexcount(1)]
void main(
	point GSInput input[1],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	element.normal = float3(0, 0, -1);
	element.tangent = float3(0, 1, 0);
	element.bitangent = float3(1, 0, 0);

	element.position = input[0].position + float4(0,0,0,1);
	element.uv = float2(0,0);
	element.fragpos = input[0].fragpos;
	output.Append(element);

	element.position = input[0].position + float4(1, 0, 0, 1);
	element.uv = float2(1, 0);
	output.Append(element);

	element.position = input[0].position + float4(0, 1, 0, 1);
	element.uv = float2(0, 1);
	output.Append(element);

	element.position = input[0].position + float4(1, 1, 0, 1);
	element.uv = float2(1, 1);
	output.Append(element);

	/*for (uint i = 0; i < 4; i++)
	{
		GSOutput element;
		element.position = input[i].position;
		element.uv = input[i].uv;
		element.normal = input[i].normal;
		element.tangent = input[i].tangent;
		element.bitangent = input[i].bitangent;
		element.fragpos = input[i].fragpos;
		output.Append(element);
	}*/

}