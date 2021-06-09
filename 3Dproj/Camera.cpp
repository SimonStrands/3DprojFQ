#include "Camera.h"
#include <iostream>
//git

Camera::Camera(Graphics *&gfx, Mouse* mus)
{
	this->Pcbd = gfx->getPcb();
	this->Vcbd = gfx->getVcb();
	this->mus = mus;

	this->speed = 10.0f;
	this->mouseSensitivity = 5.0f;
	this->xCamPos = 0.f;
	this->yCamPos = 0.0;
	this->zCamPos = 10.0f;
	this->xCamRot = 0;
	this->yCamRot = 0;
}

Camera::~Camera()
{
}

void Camera::updateCamera(float dt)
{	
	handleEvent(dt);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		xCamPos,yCamPos,zCamPos,1.0f
	);
	rotaiton(viewMatrix);

	
	//just add it to the pixel shader
	movement();
	Vcbd->view.element = viewMatrix;

	DirectX::XMFLOAT4X4 p;
	DirectX::XMStoreFloat4x4(&p, viewMatrix);
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
bool once = false;
void Camera::handleEvent(float dt)
{
	translation = DirectX::XMFLOAT3(0, 0, 0);
	//movement
	if (getkey('W') && !once) {
		translation = DirectX::XMFLOAT3(0, 0, (float)dt);
		Translate(dt);
	}
	if (getkey('D')) {
		translation = DirectX::XMFLOAT3((float)dt, 0, 0);
		Translate(dt);
	}
	if (getkey('S')) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
		Translate(dt);
	}
	if (getkey('A')) {
		translation = DirectX::XMFLOAT3(-(float)dt, 0, 0);
		Translate(dt);
	}
	if (GetKeyState(VK_SPACE) & 0x8000) {
		yCamPos -= movementspeed * (float)dt;
	}
	if (GetKeyState(VK_SHIFT) & 0x8000) {
		yCamPos += movementspeed * (float)dt;
	}
	
	//rot
	if (!mus->getMouseActive()) {
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			xCamRot += mouseSensitivity * (float)dt;
		}
		if (GetKeyState(VK_LEFT) & 0x8000) {
			xCamRot -= mouseSensitivity * (float)dt;
		}
		if (GetKeyState(VK_UP) & 0x8000) {
			yCamRot += mouseSensitivity * (float)dt;
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			yCamRot -= mouseSensitivity * (float)dt;
		}
	}
	else {
		xCamRot += mus->getDeltaPos().x * mus->getSense() * (float)dt;
		yCamRot -= mus->getDeltaPos().y * mus->getSense() * (float)dt;
	}
}

void Camera::Translate(float dt)
{
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(yCamRot, xCamRot, 0.0) *
		DirectX::XMMatrixScaling(1, 1, 1)//this line is not neccessary but I am afraid to break things
	));
	vec2 trans(translation.x, translation.z);
	trans.Normalize();
	xCamPos -= trans.x * movementspeed * dt;
	zCamPos -= trans.y * movementspeed * dt;
}
