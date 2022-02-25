#pragma once
#include "Vec.h"
#include "rotation.h"
//git

//whatTypeOfLight so I don't ahve to do a dynamic_ thing
enum wTofL {
	SPOT, DIR
};

class Light {
public:
	Light(vec3 pos);
	virtual ~Light();
	vec3& getRotation();
	vec3& getPos();
private:
	vec3 pos;
};


class SpotLight : public Light{
public:
	SpotLight(vec3 pos, vec3 rot = vec3(0,0,1));
	virtual ~SpotLight();
	vec3& getRotation();
	void changeRot(vec3 rot);
	void addRot(vec3 rot);
	DirectX::XMMATRIX getLightView();
	const wTofL whatOfLight();
protected:
	wTofL flag;
private:
	vec3 rot;
};

class DirLight : public SpotLight {
public:
	DirLight(vec3 pos, vec3 rot = vec3(0, 0, 1), float W = 50, float H = 50);
private:
	float width;
	float height;
};