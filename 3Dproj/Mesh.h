#pragma once
#include "ReadObjFile.h"
#include "CreateBuffer.h"
#include "Graphics.h"

class Mesh {
public:
	Mesh(Graphics*& gfx, std::string fileToLoad, ID3D11ShaderResourceView* def);
	virtual ~Mesh();
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