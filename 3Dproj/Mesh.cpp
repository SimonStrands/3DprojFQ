#include "Mesh.h"

MeshObj::MeshObj(Graphics*& gfx, std::vector<vertex> vertecies, Material &material)
{
	this->HS = nullptr;
	this->DS = nullptr;
	this->nrOfVertexes = (int)vertecies.size();
	CreateVertexBuffer(gfx->getDevice(), vertecies, this->vertexBuffer);
	this->matrial = material;
	CreateVertexConstBuffer(gfx, this->Pg_pConstantBuffer);
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
	return this->matrial.texSRVPS;
}

int& MeshObj::getNrOfVertex()
{
	return nrOfVertexes;
}


//ns is in the 4:th element of ks
void MeshObj::getKdKaKsNs(float(&kd)[4], float(&ka)[4], float(&ks)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = this->matrial.Kd[i];
		ka[i] = this->matrial.Ka[i];
		ks[i] = this->matrial.Ks[i];
	}
	ks[3] = matrial.Ns;
	kd[3] = 1.f;
	ka[3] = 1.f;
}

void MeshObj::draw(ID3D11DeviceContext*& immediateContext, bool sm)
{
	UINT offset = 0;
	static UINT strid = sizeof(vertex);

	immediateContext->DSSetShaderResources(0, 1, this->matrial.texSRVDS);
	immediateContext->PSSetShaderResources(0, 4, this->matrial.texSRVPS);//denna �r fel p� n�got s�tt
	immediateContext->PSSetConstantBuffers(0, 1, &this->Pg_pConstantBuffer);
	immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &strid, &offset);
	immediateContext->Draw(this->nrOfVertexes, 0);
}

void MeshObj::SetShaders(ID3D11VertexShader* VS)
{
	this->VS = VS;
}

void MeshObj::SetShaders(ID3D11VertexShader* VS, ID3D11PixelShader* PS)
{
	this->VS = VS;
	this->PS = PS;
}

void MeshObj::SetShaders(ID3D11HullShader* HS, ID3D11DomainShader* DS)
{
	this->HS = HS;
	this->DS = DS;
}

void MeshObj::SetShader(ID3D11DeviceContext*& immediateContext, int flag)
{
	if (flag == 0) {
		immediateContext->VSSetShader(this->VS, nullptr, 0);
		immediateContext->PSSetShader(this->PS, nullptr, 0);
		immediateContext->DSSetShader(this->DS, nullptr, 0);
		immediateContext->HSSetShader(this->HS, nullptr, 0);
	}
	else if (flag == 1) {
		immediateContext->VSSetShader(this->VS, nullptr, 0);
		immediateContext->DSSetShader(this->DS, nullptr, 0);
		immediateContext->HSSetShader(this->HS, nullptr, 0);
	}


}

void MeshObj::updatePS(Graphics*& gfx)
{
	getKdKaKsNs(gfx->getPcb()->kd.element, gfx->getPcb()->ka.element, gfx->getPcb()->ks.element);
	D3D11_MAPPED_SUBRESOURCE resource;
	gfx->get_IC()->Map(Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getPcb(), sizeof(Pcb));
	gfx->get_IC()->Unmap(Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

Material& MeshObj::getMatrial()
{
	return this->matrial;
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