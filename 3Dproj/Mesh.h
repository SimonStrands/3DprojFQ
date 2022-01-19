#pragma once
#include "ReadObjFile.h"
#include "CreateBuffer.h"
#include "object.h"
#include "Graphics.h"
#include "Bone.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//for obj only
class MeshObj {
public:
	MeshObj(Graphics*& gfx, std::string fileToLoad, ID3D11ShaderResourceView** def);
	virtual ~MeshObj();
	ID3D11Buffer*& getVertexBuffer();
	ID3D11ShaderResourceView** getTextures();
	int& getNrOfVertex();
	int& getNrOfTextures();
	void getKdKa(float (&kd)[4], float (&ka)[4]);
private:
	int nrOfTextures;
	int nrOfVertexes;
	bool defTexture[3] = { false, false, false };
	ID3D11ShaderResourceView** texSRV;
	ID3D11Buffer* vertexBuffer;
	float ka[3];
	float kd[3];
};

class Mesh {
public:
	Mesh(Graphics *& gfx, std::vector<vertex> vertecies, std::vector<DWORD>& indices);
	Mesh(Graphics *& gfx, std::vector<BoneVertex> vertecies, std::vector<DWORD>& indices);
	Mesh(const Mesh& mesh);
	virtual ~Mesh();
	void Draw(ID3D11DeviceContext*& immediateContext);
	const int getNrOfVertexes();
	//const int getNrOfIndecies(); will fix
	int nrOfIndecies;
private:
	int nrOfTextures;
	int nrOfVertexes;
	
	bool defTexture[3] = { false, false, false };
	ID3D11ShaderResourceView** texSRV;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indicesBuffer;
	ID3D11DeviceContext* ctx;
};
