#pragma once
#include <d3d11.h>
#include "Vec.h"
#include "Graphics.h"

class object {
public:
	object();
	object(vec3 pos);
	virtual ~object();
	virtual void draw(ID3D11DeviceContext*& immediateContext) = 0;

	vec3 getPos();
	const vec3 getRot();
	const vec3 getScale();
	const vec3 getPoint();

	void changePos(vec3 pos);
	void changeRot(vec3 pos);
	void changeScale(vec3 pos);
	void changePoint(vec3 point);
	void addPoint(vec3 point);
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

	void updateVertexShader(Graphics*& gfx);
	void updatePixelShader(Graphics*& gfx);
protected:
	float normalMap;
	float kd[4];
	float ka[4];
private:
	vec3 pos;
	vec3 rot;
	vec3 scale;
	vec3 rPoint;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
};

