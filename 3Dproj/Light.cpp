#include "Light.h"
//git
PointLight::PointLight(vec3 pos):
pos(pos)
{
}

vec3 &PointLight::getPos()
{
	return this->pos;
}


