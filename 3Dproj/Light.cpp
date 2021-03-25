#include "Light.h"
//git
PointLight::PointLight(vec3 pos):
Light(pos)
{
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
