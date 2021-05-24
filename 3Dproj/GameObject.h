#pragma once
#include "object.h"
#include <vector>
#include "vertex.h"
#include <string>
#include "Vec.h"
#include "Mesh.h"

//#thisNeedFix
//git
class Graphics;
class GameObject : public object{
public:
	GameObject(Mesh *file, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), std::string otherTex = "");
	virtual ~GameObject();

	void draw(ID3D11DeviceContext*& immediateContext);
private:
	//object data
	ID3D11ShaderResourceView* special;
	Mesh *mesh;
	ID3D11Buffer* vertexBuffer;
};