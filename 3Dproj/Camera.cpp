#include "Camera.h"
#include <iostream>
//git

Camera::Camera(Graphics *&gfx)
{
	this->gfx = gfx;
	this->Pcbd = gfx->getPcb();
	this->Vcbd = gfx->getVcb();

	this->speed = 10.0f;
	this->mouseSensitivity = 5.0f;
	this->xCamPos = -0.5f;
	this->yCamPos = 0.0;
	this->zCamPos = 2.0f;
	this->xCamRot = 0;
	this->yCamRot = 0;
}

Camera::~Camera()
{
}

void Camera::updateCamera(DeltaTime dt)
{	
	handleEvent(dt);
	DirectX::XMMATRIX viewMatrix(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		xCamPos,yCamPos,zCamPos,1.0f
	);
	movement();
	rotaiton(viewMatrix);
	Vcbd->view.element = viewMatrix;
}

vec3 Camera::getPos()
{
	return vec3(xCamPos, yCamPos, zCamPos);
}

void Camera::rotaiton(DirectX::XMMATRIX &matrix)
{
	XRotation(matrix, xCamRot);
	YRotation(matrix, yCamRot);
}

void Camera::movement()
{
	Pcbd->cameraPos.element[0] = -xCamPos;
	Pcbd->cameraPos.element[1] = -yCamPos;
	Pcbd->cameraPos.element[2] = -zCamPos;
}

void Camera::handleEvent(DeltaTime dt)
{
	if (getkey('W')) {
		zCamPos -= speed * (float)dt.dt();
	}
	if (getkey('D')) {
		xCamPos -= speed * (float)dt.dt();
	}
	if (getkey('S')) {
		zCamPos += speed * (float)dt.dt();
	}
	if (getkey('A')) {
		xCamPos += speed * (float)dt.dt();
	}
	if (GetKeyState(VK_SPACE) & 0x8000) {
		yCamPos -= mouseSensitivity * (float)dt.dt();
	}
	if (GetKeyState(VK_SHIFT) & 0x8000) {
		yCamPos += mouseSensitivity * (float)dt.dt();
	}
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		xCamRot += mouseSensitivity * (float)dt.dt();
	}
	if (GetKeyState(VK_LEFT) & 0x8000) {
		xCamRot -= mouseSensitivity * (float)dt.dt();
	}
	if (GetKeyState(VK_UP) & 0x8000) {
		yCamRot += mouseSensitivity * (float)dt.dt();
	}
	if (GetKeyState(VK_DOWN) & 0x8000) {
		yCamRot -= mouseSensitivity * (float)dt.dt();
	}
}
