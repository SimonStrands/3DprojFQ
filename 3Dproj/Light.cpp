#include "Light.h"
//git
SpotLight::SpotLight(vec3 pos, vec3 rot):
Light(pos)
{
	this->rot = rot;
}

SpotLight::~SpotLight()
{
}

vec3 SpotLight::getRotation()
{
	return this->rot;
}

void SpotLight::changeRot(vec3 rot)
{
	this->rot = rot;
}

void SpotLight::addRot(vec3 rot)
{
	this->rot = this->rot + rot;
}

DirectX::XMMATRIX SpotLight::getLightView()
{
	DirectX::XMMATRIX temp(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-getPos().x, -getPos().y, -getPos().z, 1.0f
	);
	XRotation(temp, rot.x);
	YRotation(temp, rot.y);
	return temp;
}

Light::Light(vec3 pos)
{
	this->pos = pos;
}

Light::~Light()
{
}

vec3& Light::getPos()
{
	return this->pos;
}

