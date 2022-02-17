#pragma once
#include "GameObject.h"
#include "plHelper.h"

class DynamicCube : public GameObject {
public:
	DynamicCube(ModelObj* model, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void draw(ID3D11DeviceContext*& immediateContext);
	ID3D11UnorderedAccessView** getUAVs();
private:
	bool initCubeMapping(Graphics*& gfx);
	ID3D11ShaderResourceView* CubeResV;
	ID3D11Texture2D* CubeTex;
	ID3D11RenderTargetView** CubeRTV;
	ID3D11UnorderedAccessView** UAVs;
};