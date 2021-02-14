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
	object(std::string file, Graphics &gfx, std::string texture = "stripestest.png", vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~object();
	const vec3 getPos();
	const vec3 getRot();
	const vec3 getScale();

	void changePos(vec3 pos);
	void changeRot(vec3 pos);
	void changeScale(vec3 pos);

	ID3D11Buffer*& getVertexBuffer();
	ID3D11Buffer*& getVertexConstBuffer();
	int& getNrOfVertex();

	ID3D11ShaderResourceView** texSRV = new ID3D11ShaderResourceView*[2];
	std::string fileName[2];//color, normal

	//debug and should be deleted before realease
	float &getxRot();

private:
	//object data
	vec3 pos;
	vec3 rot;
	vec3 scale;
	int nrOfVertexes;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* Vg_pConstantBuffer;
	ID3D11Buffer* Pg_pConstantBuffer;
};