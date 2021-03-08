#pragma once
#include <d3d11.h>
#include "Vec.h"

class object {
public:
	object();
	object(vec3 pos);
	virtual ~object();
	virtual void draw(ID3D11DeviceContext*& immediateContext) = 0;

	vec3 getPos();
	const vec3 getRot();
	const vec3 getScale();

	void changePos(vec3 pos);
	void changeRot(vec3 pos);
	void changeScale(vec3 pos);
	void addPos(vec3 pos);
	void addRot(vec3 rot);
	void addScale(vec3 scale);

	//debug
	float& getxRot();
	float& getxPos();
	float& getzPos();
private:
	vec3 pos;
	vec3 rot;
	vec3 scale;
};