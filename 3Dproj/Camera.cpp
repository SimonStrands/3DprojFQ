#include "Camera.h"
#include <iostream>
//git

Camera::Camera(Graphics *&gfx, Mouse* mus, Keyboard* keyboard, vec3 pos, vec3 rot)
{
	this->Lcbd = gfx->getLightconstbufferforCS();
	this->Vcbd = gfx->getVertexconstbuffer();
	this->mus = mus;
	this->keyboard = keyboard;
	this->mouseSensitivity = 5.0f;
	this->xCamPos = pos.x;
	this->yCamPos = pos.y;
	this->zCamPos = pos.z;

	this->xCamRot = rot.x;
	this->yCamRot = rot.y;
	this->zCamRot = rot.z;
	this->calcFURVectors();
}

Camera::~Camera()
{
}

void Camera::updateCamera() {
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-xCamPos, -yCamPos, -zCamPos, 1.0f
	);
	rotaiton(viewMatrix);


	//just add it to the pixel shader
	movement();
	Vcbd->view.element = viewMatrix;
}
void Camera::updateCamera(float dt)
{	
	handleEvent(dt);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-xCamPos,-yCamPos,-zCamPos,1.0f
	);
	rotaiton(viewMatrix);

	
	//just add it to the pixel shader
	movement();
	Vcbd->view.element = viewMatrix;

}

vec3 Camera::getPos()
{
	return vec3(xCamPos, yCamPos, zCamPos);
}

vec3 Camera::getRot()
{
	return vec3(xCamRot, yCamRot, 0);
}

void Camera::calcFURVectors()
{
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-xCamPos, -yCamPos, -zCamPos, 1.0f
	);
	rotaiton(viewMatrix);


	//just add it to the pixel shader
	movement();
	DirectX::XMFLOAT4X4 theViewMatrix;
	DirectX::XMStoreFloat4x4(&theViewMatrix, viewMatrix);
	FUL[0] = vec3(theViewMatrix._13, theViewMatrix._23, theViewMatrix._33);
	FUL[1] = vec3(theViewMatrix._12, theViewMatrix._22, theViewMatrix._32);
	FUL[2] = vec3(theViewMatrix._11, theViewMatrix._21, theViewMatrix._31);
}

vec3 Camera::getForwardVec()
{
	return FUL[0];
}

vec3 Camera::getUpVector()
{
	return FUL[1];
}

vec3 Camera::getRightVector()
{
	return FUL[2];
}

void Camera::getViewFrustoms(vec3 frustoms[])
{
	vec3 nearCenter = getForwardVec() * nearDist;
	vec3 farCenter = getPos() - getForwardVec() * farDist;

	float nearHeight = 2 * tan(fov / 2) * nearDist;
	float farHeight = 2 * tan(fov / 2) * farDist;
	float nearWidth = nearHeight * ratio;
	float farWidth = farHeight * ratio;

	//left, right, up, down 
	frustoms[0] = vec3(nearCenter - getRightVector() * ((float)nearWidth * 0.5f)).Normalize();
	frustoms[1] = vec3(nearCenter + getRightVector() * ((float)nearWidth * 0.5f)).Normalize();
	frustoms[2] = vec3(nearCenter + getUpVector() * ((float)nearHeight * 0.5f)).Normalize();
	frustoms[3] = vec3(nearCenter - getUpVector() * ((float)nearHeight * 0.5f)).Normalize();
}

void Camera::setRotation(vec3 newRot)
{
	this->xCamRot = newRot.x;
	this->yCamRot = newRot.y;
}

void Camera::setPosition(vec3 newpos)
{
	this->xCamPos = newpos.x;
	this->yCamPos = newpos.y;
	this->zCamPos = newpos.z;
}

void Camera::rotateCamera(vec3 rotation)
{
	xCamRot += rotation.x * mus->getSense();
	yCamRot -= rotation.y * mus->getSense();
	std::cout << rotation.x << "x" << std::endl;
	std::cout << rotation.y << "x" << std::endl;
}

void Camera::setData(float FOVRadians, float viewRatio, float nearDist, float farDist)
{
	this->ratio = viewRatio;
	this->fov = FOVRadians;
	this->nearDist = nearDist;
	this->farDist = farDist;
}

void Camera::rotaiton(DirectX::XMMATRIX& matrix, float xRot, float yRot) {
	XRotation(matrix, xRot);
	YRotation(matrix, yRot);
}

void Camera::rotaiton(DirectX::XMMATRIX &matrix)
{
	XRotation(matrix, xCamRot);
	YRotation(matrix, yCamRot);
}

void Camera::movement()
{
	Lcbd->cameraPos.element[0] = xCamPos;
	Lcbd->cameraPos.element[1] = yCamPos;
	Lcbd->cameraPos.element[2] = zCamPos;
}
bool once = false;
void Camera::handleEvent(float dt)
{
	translation = DirectX::XMFLOAT3(0, 0, 0);
	//movement
	if (keyboard->isKeyPressed('W') && !once) {
		translation = DirectX::XMFLOAT3(0, 0, -(float)dt);
		Translate(dt);
	}
	if (keyboard->isKeyPressed('D')) {
		translation = DirectX::XMFLOAT3(-(float)dt, 0, 0);
		Translate(dt);
	}
	if (keyboard->isKeyPressed('S')) {
		translation = DirectX::XMFLOAT3(0, 0, (float)dt);
		Translate(dt);
	}
	if (keyboard->isKeyPressed('A')) {
		translation = DirectX::XMFLOAT3((float)dt, 0, 0);
		Translate(dt);
	}
	if (keyboard->isKeyPressed(VK_SPACE)) {
		yCamPos += movementspeed * (float)dt;
	}
	if (keyboard->isKeyPressed(VK_SHIFT)) {
		yCamPos -= movementspeed * (float)dt;
	}
	
	//rot
	if (!mus->getMouseActive()) {
		if (keyboard->isKeyPressed(VK_RIGHT)) {
			xCamRot += mouseSensitivity * (float)dt;
		}
		if (keyboard->isKeyPressed(VK_LEFT)) {
			xCamRot -= mouseSensitivity * (float)dt;
		}
		if (keyboard->isKeyPressed(VK_UP)) {
			yCamRot += mouseSensitivity * (float)dt;
		}
		if (keyboard->isKeyPressed(VK_DOWN)) {
			yCamRot -= mouseSensitivity * (float)dt;
		}
	}
	//else {
	//	xCamRot += mus->getDeltaPos().x * mus->getSense() * (float)dt;
	//	yCamRot -= mus->getDeltaPos().y * mus->getSense() * (float)dt;
	//}
}

void Camera::Translate(float dt)
{
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(yCamRot, xCamRot, 0) *
		DirectX::XMMatrixScaling(1, 1, 1)//this line is not neccessary but I am afraid to break things
	));
	vec2 trans(translation.x, translation.z);
	trans.Normalize();
	xCamPos -= trans.x * movementspeed * dt;
	zCamPos -= trans.y * movementspeed * dt;
}
