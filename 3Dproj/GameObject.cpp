#include "GameObject.h"



GameObject::GameObject(Mesh *file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->Vg_pConstantBuffer = nullptr;
	this->Pg_pConstantBuffer = nullptr;
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	mesh = file;
	CreateVertexConstBuffer(gfx, Vg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, Pg_pConstantBuffer);
	float kd[4];
	float ka[4];
	file->getKdKa(kd, ka);
	for (int i = 0; i < 4; i++) {
		
		gfx->getPcb()->ka.element[i] = ka[i];
		printf("%f %f\n", ka[i], gfx->getPcb()->ka.element[i]);
		gfx->getPcb()->kd.element[i] = kd[i];
	}
	
}

GameObject::~GameObject()
{
	//delete mesh;
	if (Vg_pConstantBuffer != nullptr) {
		Vg_pConstantBuffer->Release();
	};
	if (Pg_pConstantBuffer != nullptr) {
		Pg_pConstantBuffer->Release();
	};
}

ID3D11Buffer*& GameObject::getVertexConstBuffer()
{
	return this->Vg_pConstantBuffer;
}

ID3D11Buffer*& GameObject::getPixelConstBuffer()
{
	return this->Pg_pConstantBuffer;
}

void GameObject::draw(ID3D11DeviceContext*& immediateContext)
{
	static UINT strid = sizeof(vertex);
	static UINT offset = 0;
	immediateContext->PSSetShaderResources(0, mesh->getNrOfTextures(), mesh->getTextures());
	immediateContext->VSSetConstantBuffers(0, 1, &Vg_pConstantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	immediateContext->IASetVertexBuffers(0, 1, &mesh->getVertexBuffer(), &strid, &offset);
	immediateContext->Draw(mesh->getNrOfVertex(), 0);
}