#pragma once
#include "GameObject.h"
#include "plHelper.h"

struct DCCB : public CB {
	struct {
		float element[4];
	}cameraPos;
};

class DynamicCube : public GameObject {
public:
	DynamicCube(ModelObj* model, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void draw(ID3D11DeviceContext*& immediateContext);
	ID3D11UnorderedAccessView** getUAVs();
	ID3D11ComputeShader *getCSShader();
	void update(vec3 camPos, Graphics*& gfx);
private:
	bool initCubeMapping(Graphics*& gfx);
	ID3D11ShaderResourceView* CubeResV;
	ID3D11Texture2D* CubeTex;
	ID3D11UnorderedAccessView** UAVs;
	ID3D11ComputeShader* CSShader;

	ID3D11Buffer* CBbuffer;
	DCCB ConstBufferElements = {};
};