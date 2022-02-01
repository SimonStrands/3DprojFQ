cbuffer lightOCameraCBuf : register (b3)
{
	float4 lightPos;
	float4 cameraPos;
	float4 lightColor;
	row_major matrix projection;
	row_major matrix lightView[2];//right now max number of lights is 1
	//int nrOfLight; LATER
}