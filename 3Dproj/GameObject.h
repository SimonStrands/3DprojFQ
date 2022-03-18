#pragma once
#include <vector>
#include <string>
#include "vertex.h"
#include "object.h"

#include "Model.h"

//#thisNeedFix
//git
class GameObject : public object{
public:
	GameObject(ModelObj*file, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), std::string otherTex = "");
	virtual ~GameObject();
	void Updateshaders(Graphics*& gfx, bool vertex = true, bool pixel = true);
	void draw(Graphics*& gfx, bool sm = false);
	void drawDebug(Graphics*& gfx, bool sm = false);
	void drawDefTest(ID3D11DeviceContext*& immediateContext);
	void setTesselation(bool tess, Graphics*& gfx);
	//0 = lowest 1 = highest
	void getBox(DirectX::XMVECTOR theReturn[]);
	const bool isDrawed();
	void clearDrawed();
protected:
	ModelObj* model;
private:
	bool tess;
	bool drawed;
};