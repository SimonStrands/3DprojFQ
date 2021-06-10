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
	PointLight(vec3 pos, vec3 rot = vec3(0,0,1));
	vec3 getRotation();
	void changeRot(vec3 rot);
	void addRot(vec3 rot);
private:
	vec3 rot;
};