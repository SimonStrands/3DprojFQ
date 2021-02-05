#pragma once
#include <vector>
#include "vertex.h"
#include <string>
#include "Vec.h"
#include <d3d11.h>

//#thisNeedFix
//git
class Graphics;
class object{
public:
	object(std::string file, Graphics &gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~object();
	const vec3 getPos();
	const vec3 getRot();
	const vec3 getScale();
	std::vector<std::vector<vertex>> &getVertecies();
	//dubg
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* Vg_pConstantBuffer;
private:
	vec3 pos;
	vec3 rot;
	vec3 scale;
	std::string fileName;
	std::vector<std::vector<vertex>> vertices; //one for all the object in one object other for the vertecies for one object
	std::vector<std::string> material; 
};
//vertices