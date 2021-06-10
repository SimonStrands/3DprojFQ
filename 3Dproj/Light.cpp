#include "Light.h"
//git
PointLight::PointLight(vec3 pos, vec3 rot):
Light(pos)
{
	this->rot = rot;
}

vec3 PointLight::getRotation()
{
	return this->rot;
}

void PointLight::changeRot(vec3 rot)
{
	this->rot = rot;
}

void PointLight::addRot(vec3 rot)
{
	this->rot = this->rot + rot;
}

Light::Light(vec3 pos)
{
	this->pos = pos;
}

vec3& Light::getPos()
{
	return this->pos;
}

void Light::spawnControllerWindow()
{

}
