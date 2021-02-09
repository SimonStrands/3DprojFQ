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
	this->xCamPos = 0.f;
	this->yCamPos = 0.0;
	this->zCamPos = 0.0f;
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
	if (GetKeyState(VK_RETURN) & 0x8000) {
		//printf("camRot: x: %f, y: %f  z: %f\n", p.m[0][0], p.m[1][1], p.m[2][2]);
		printf("campos: %f, %f %f\n", xCamPos, yCamPos, zCamPos);
	}
	

	

	//Vcbd->view.element = viewMatrix;
	/*DirectX::XMVECTOR forwardBaseVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR lookVector = DirectX::XMVector3Transform(forwardBaseVector,
		DirectX::XMMatrixRotationRollPitchYaw(yCamRot, xCamRot, 0.0f)
	);
	DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(xCamPos, yCamPos, zCamPos));
	DirectX::XMVECTOR camTarget = DirectX::XMVectorSet((xCamPos + lookVector.m128_f32[0]), (yCamPos + lookVector.m128_f32[1]), (zCamPos + lookVector.m128_f32[2]), 1);
	//DirectX::XMVECTOR camTarget(camPosition.m128_f32[0] + lookVector.m128_f32[0]);

	Vcbd->view.element = DirectX::XMMatrixLookAtRH(camPosition, camTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	*/

	/*
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(xCamRot, yCamRot, 0.0) *
		DirectX::XMMatrixScaling(speed, speed, speed)
	));
	
	xCamPos += xCamPos + translation.x;
	yCamPos += yCamPos + translation.y;
	zCamPos += zCamPos + translation.z;

	DirectX::XMMATRIX changes = DirectX::XMMatrixTranslation(-xCamPos, -yCamPos, -zCamPos)*
	DirectX::XMMatrixRotationRollPitchYaw(-xCamRot, -yCamRot, 0.0f);
	
	viewMatrix = changes * viewMatrix;
	*/
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
