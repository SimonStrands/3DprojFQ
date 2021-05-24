#include "GameObject.h"



GameObject::GameObject(Mesh *file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	mesh = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreatePixelConstBuffer(gfx, this->getPixelConstBuffer());
	file->getKdKa(this->kd, this->ka);
	this->normalMap = 1.f;

	this->special = gfx->special();
}

GameObject::~GameObject()
{

}

void GameObject::draw(ID3D11DeviceContext*& immediateContext)
{
	static UINT strid = sizeof(vertex);
	static UINT offset = 0;
	if (special == nullptr) {
		immediateContext->PSSetShaderResources(0, mesh->getNrOfTextures(), mesh->getTextures());
	}
	else {
		immediateContext->PSSetShaderResources(0, 1, &special);
		immediateContext->PSSetShaderResources(1, 1, &special);
		immediateContext->PSSetShaderResources(2, 1, &special);
	}
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->PSSetConstantBuffers(0, 1, &this->getPixelConstBuffer());
	immediateContext->IASetVertexBuffers(0, 1, &mesh->getVertexBuffer(), &strid, &offset);
	immediateContext->Draw(mesh->getNrOfVertex(), 0);
}
