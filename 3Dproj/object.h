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

	ID3D11Buffer*& getVertexConstBuffer();
	ID3D11Buffer*& getPixelConstBuffer();
	float &normalMapping();
	void getKdKa(float kd[4], float ka[4]);

	//debug
	float& getxRot();

	float& getxPos();
	float& getyPos();
	float& getzPos();
protected:
	float normalMap;
	float kd[4];
	float ka[4];
private:
	vec3 pos;
	vec3 rot;
	vec3 scale;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
};