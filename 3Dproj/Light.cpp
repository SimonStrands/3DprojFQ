#include "Light.h"
//git
SpotLight::SpotLight(vec3 pos, vec3 rot):
Light(pos)
{
	this->rot = rot;
	this->flag = wTofL::SPOT;
}

SpotLight::~SpotLight()
{
}

vec3& SpotLight::getRotation()
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

const wTofL SpotLight::whatOfLight()
{
	return this->flag;
}

Light::Light(vec3 pos)
{
	this->pos = pos;
}

Light::~Light()
{
}

vec3& Light::getRotation()
{
	SpotLight* SL = dynamic_cast<SpotLight*>(this);
	if (SL != nullptr) {
		return SL->getRotation();
	}
	vec3 a(0, 0, 0);
	return a;
}

vec3& Light::getPos()
{
	return this->pos;
}

DirLight::DirLight(vec3 pos, vec3 rot, float W, float H):
	SpotLight(pos, rot)
{
	this->flag = wTofL::DIR;
	this->width = W;
	this->height = H;
}
