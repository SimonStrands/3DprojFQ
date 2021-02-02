#include "rotation.h"
//git
void XRotation(DirectX::XMMATRIX &Matrix, float rot)
{
	//X rotation
	DirectX::XMMATRIX Mrot(
		 std::cos(rot), 0.0f, std::sin(rot), 0.0f ,
		 0.0f,			 1.0f,	0.0f,			 0.0f ,
		 -std::sin(rot),0.0f, std::cos(rot), 0.0f ,
		 0.0f,			 0.0f,			 0.0f,  1.0f 
);
	Matrix = Matrix * Mrot;
}

void YRotation(DirectX::XMMATRIX &Matrix, float rot)
{
	DirectX::XMMATRIX Mrot(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(rot), std::sin(rot), 0.0f,
		0.0f, -std::sin(rot), std::cos(rot), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = Matrix * Mrot;
}

void ZRotation(DirectX::XMMATRIX &Matrix, float rot)
{
	DirectX::XMMATRIX Mrot(
		std::cos(rot), -std::sin(rot), 0.0f, 0.0f,
		std::sin(rot), std::cos(rot), 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	Matrix = Matrix * Mrot;
}
