#pragma once
#include "CreateBuffer.h"
#include "Graphics.h"
#include "Bone.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//for obj only
class MeshObj {
public:
	MeshObj(Graphics*& gfx, std::vector<vertex> vertecies, Material &material);
	void begone();
	virtual ~MeshObj();
	ID3D11Buffer*& getVertexBuffer();
	ID3D11ShaderResourceView** getTextures();
	int& getNrOfVertex();
	int& getNrOfTextures();
	void getKdKa(float (&kd)[4], float (&ka)[4]);
	void draw(ID3D11DeviceContext*& immediateContext);
	void SetShaders(ID3D11VertexShader* VS, ID3D11PixelShader* PS);
	void SetShader(ID3D11DeviceContext*& immediateContext);
	//DEBUG
	ID3D11PixelShader* PS;
	ID3D11VertexShader* VS;
private:
	int nrOfVertexes;
	bool defTexture[4] = { false, false, false, false };
	Material matrial;
	ID3D11Buffer* vertexBuffer;
	//ID3D11PixelShader* PS;
	//ID3D11VertexShader* VS;
	
};

/*class Mesh {
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
};*/
