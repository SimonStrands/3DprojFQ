#include "Mesh.h"

MeshObj::MeshObj(Graphics*& gfx, std::vector<vertex> vertecies, Material &material)
{
	this->nrOfVertexes = (int)vertecies.size();
	CreateVertexBuffer(gfx->getDevice(), vertecies, this->vertexBuffer);
	this->matrial = material;
}

void MeshObj::begone()
{
	if (this->vertexBuffer != nullptr) {
		this->vertexBuffer->Release();
	}
	matrial.begone();
}

MeshObj::~MeshObj()
{
}

ID3D11Buffer*& MeshObj::getVertexBuffer()
{
	return this->vertexBuffer;
}

ID3D11ShaderResourceView** MeshObj::getTextures()
{
	return this->matrial.texSRV;
}

int& MeshObj::getNrOfVertex()
{
	return nrOfVertexes;
}

int& MeshObj::getNrOfTextures()
{
	return this->matrial.nrOfTextures;
}

void MeshObj::getKdKa(float (&kd)[4], float (&ka)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = this->matrial.Kd[i];
		ka[i] = this->matrial.Ka[i];
	}
	kd[3] = 1.f;
	ka[3] = 1.f;
}

void MeshObj::draw(ID3D11DeviceContext*& immediateContext)
{
	UINT offset = 0;
	static UINT strid = sizeof(vertex);
	immediateContext->PSSetShaderResources(0, 4, this->matrial.texSRV);
	immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &strid, &offset);
	immediateContext->Draw(this->nrOfVertexes, 0);
}

void MeshObj::SetShaders(ID3D11VertexShader* VS, ID3D11PixelShader* PS)
{
	this->VS = VS;
	this->PS = PS;
}

void MeshObj::SetShader(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->VSSetShader(this->VS, nullptr, 0);
	immediateContext->PSSetShader(this->PS, nullptr, 0);
}


//////////////////////////////////////////////////////////////////////////////////////////

/*
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
*/