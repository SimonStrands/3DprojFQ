#include "Mesh.h"

Mesh::Mesh(Graphics*& gfx, std::string fileToLoad, ID3D11ShaderResourceView** def)
{
	nrOfVertexes = 0;
	CreateVertexBuffer(gfx, *this, fileToLoad);
	std::vector<std::string> fileNames;
	fileNames = getTextureNames(fileToLoad);
	nrOfTextures = (int)fileNames.size() - 2;
	texSRV = new ID3D11ShaderResourceView * [nrOfTextures];
	for (int i = 0; i < nrOfTextures; i++) {
		if (!CreateTexture(fileNames[i + 2], gfx->getDevice(), gfx->getTexture(), texSRV[i])) {
			if (i == 2) {
				texSRV[i] = def[1];
			}
			else {
				texSRV[i] = def[0];
			}
			
			defTexture[i] = true;
		}
	}
	std::istringstream a;
	a.str(fileNames[0]);
	a >> ka[0] >> ka[1] >> ka[2];
	a.clear();
	a.str(fileNames[1]);
	a >> kd[0] >> kd[1] >> kd[2];
}

Mesh::~Mesh()
{
	if (vertexBuffer != nullptr) {
		vertexBuffer->Release();
	}
	// why wont this delete
	for (int i = 0; i < nrOfTextures; i++) {
		if (texSRV[i] != nullptr && !defTexture[i]) {
			texSRV[i]->Release();
		}
	}
	delete[] texSRV;
}

ID3D11Buffer*& Mesh::getVertexBuffer()
{
	return this->vertexBuffer;
}

ID3D11ShaderResourceView** Mesh::getTextures()
{
	return this->texSRV;
}

int& Mesh::getNrOfVertex()
{
	return nrOfVertexes;
}

int& Mesh::getNrOfTextures()
{
	return nrOfTextures;
}

void Mesh::getKdKa(float (&kd)[4], float (&ka)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = this->kd[i];
		ka[i] = this->ka[i];
	}
	kd[3] = 1.f;
	ka[3] = 1.f;
}
