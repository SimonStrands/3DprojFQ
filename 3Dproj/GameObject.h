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
	void draw(ID3D11DeviceContext*& immediateContext, bool sm = false);
	void drawDefTest(ID3D11DeviceContext*& immediateContext);
	ModelObj* model;
private:
	//object data
	//ModelObj* model;
};