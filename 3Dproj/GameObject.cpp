#include "GameObject.h"

GameObject::GameObject(MeshObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	mesh = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreatePixelConstBuffer(gfx, this->getPixelConstBuffer());
	file->getKdKa(this->kd, this->ka);
	if (mesh->getTextures()[2] == nullptr) {
		this->normalMap = 0.f;
	}
	else {
		this->normalMap = 1.f;
	}
}

GameObject::~GameObject()
{
	if (vertexBuffer != nullptr) {
		vertexBuffer->Release();
	}
	
}
static bool once = false;
void GameObject::draw(ID3D11DeviceContext*& immediateContext)
{
	static UINT strid = sizeof(vertex);
	static UINT offset = 0;

	if (normalMap < 0.5) {
		immediateContext->PSSetShaderResources(0, mesh->getNrOfTextures() - 1, mesh->getTextures());
	}
	else {
		immediateContext->PSSetShaderResources(0, mesh->getNrOfTextures(), mesh->getTextures());
	}
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->PSSetConstantBuffers(0, 1, &this->getPixelConstBuffer());
	immediateContext->IASetVertexBuffers(0, 1, &mesh->getVertexBuffer(), &strid, &offset);
	immediateContext->Draw(mesh->getNrOfVertex(), 0);
}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex, bool pixel)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
	if (pixel) {
		this->updatePixelShader(gfx);
	}
}
