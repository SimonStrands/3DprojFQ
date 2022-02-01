cbuffer lightOCameraCBuf : register (b6)
{
	float4 cameraPos;
	float4 lightColor;
	row_major matrix projection;
	row_major matrix lightView[6];//right now max number of lights is 6
	//int nrOfLight; LATER
}