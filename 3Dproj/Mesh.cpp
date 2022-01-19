#include "Mesh.h"

MeshObj::MeshObj(Graphics*& gfx, std::string fileToLoad, ID3D11ShaderResourceView** def)
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
				texSRV[i] = nullptr;
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

MeshObj::~MeshObj()
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

ID3D11Buffer*& MeshObj::getVertexBuffer()
{
	return this->vertexBuffer;
}

ID3D11ShaderResourceView** MeshObj::getTextures()
{
	return this->texSRV;
}

int& MeshObj::getNrOfVertex()
{
	return nrOfVertexes;
}

int& MeshObj::getNrOfTextures()
{
	return nrOfTextures;
}

void MeshObj::getKdKa(float (&kd)[4], float (&ka)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = this->kd[i];
		ka[i] = this->ka[i];
	}
	kd[3] = 1.f;
	ka[3] = 1.f;
}


//////////////////////////////////////////////////////////////////////////////////////////


Mesh::Mesh(Graphics*& gfx, std::vector<vertex> vertecies, std::vector<DWORD>& indices)
{
	//this->nrOfVertexes = (int)vertecies.size();
	//this->nrOfIndecies = (int)indices.size();
	//CreateVertexBuffer(gfx->getDevice(), vertecies, this->vertexBuffer);
	//CreateVertexBuffer(gfx->getDevice(), indices, this->indicesBuffer, true);
}
Mesh::Mesh(Graphics*& gfx, std::vector<BoneVertex> vertecies, std::vector<DWORD>& indices) 
{
	this->nrOfVertexes = (int)vertecies.size();
	this->nrOfIndecies = (int)indices.size();
	CreateVertexBuffer(gfx->getDevice(), vertecies, this->vertexBuffer);
	printf("mybuffer at when made %p\n", vertexBuffer);
	CreateVertexBuffer(gfx->getDevice(), indices, this->indicesBuffer, true);
	vertecies;
}

Mesh::Mesh(const Mesh& mesh)
{
	this->nrOfTextures = mesh.nrOfTextures;
	this->nrOfVertexes = mesh.nrOfVertexes;
	this->nrOfIndecies = mesh.nrOfIndecies;
	this->vertexBuffer = mesh.vertexBuffer;
	this->indicesBuffer = mesh.indicesBuffer;
}

Mesh::~Mesh()
{
}

void Mesh::Draw(ID3D11DeviceContext*& immediateContext)
{
	if (nrOfIndecies < 0) {
		return;
	}
	UINT offset = 0;
	static UINT strid = sizeof(BoneVertex);
	immediateContext->IASetIndexBuffer(this->indicesBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &strid, &offset);
	immediateContext->DrawIndexed(this->nrOfIndecies, 0, 0);
}

const int Mesh::getNrOfVertexes()
{
	return this->nrOfVertexes;
}
