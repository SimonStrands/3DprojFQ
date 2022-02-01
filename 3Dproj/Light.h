#pragma once
#include "Vec.h"
#include "rotation.h"
//git

class Light {
public:
	Light(vec3 pos);
	virtual ~Light();
	vec3& getPos();
private:
	vec3 pos;
};

class SpotLight : public Light{
public:
	SpotLight(vec3 pos, vec3 rot = vec3(0,0,1));
	virtual ~SpotLight();
	vec3 getRotation();
	void changeRot(vec3 rot);
	void addRot(vec3 rot);
	DirectX::XMMATRIX getLightView();
private:
	vec3 rot;
};