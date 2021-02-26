#pragma once
#include "Vec.h"
//git
class PointLight {
public:
	PointLight(vec3 pos);
	vec3 &getPos();
private:
	vec3 pos;
};