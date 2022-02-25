static const int MAXNROFLIGHTS = 6;
cbuffer lightOCameraCBuf : register (b6)
{
	float4 cameraPos;
	float4 lightColor;
    float4 lightPos[MAXNROFLIGHTS]; //apperently rotation shits on the position
    row_major matrix projection;
    row_major matrix lightViewProj[MAXNROFLIGHTS]; 
	int nrOfLight; 
}
/*

Om jag har en matris som ser ut:
| 1  | 2 |
| 3 | 4 |
finns det något i hlsl eller directx där man gör om den till:
| 1  | 3 |
| 2 | 4 |

*/