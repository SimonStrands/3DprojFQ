#pragma once
#include "Vec.h"
//git

class Light {
public:
	Light(vec3 pos);
	vec3& getPos();
	void spawnControllerWindow();
private:
	vec3 pos;
};

class PointLight : public Light{
public:
	PointLight(vec3 pos);
	
private:

};