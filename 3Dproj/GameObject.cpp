#include "GameObject.h"

GameObject::GameObject(ModelObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	this->model = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreatePixelConstBuffer(gfx, this->getPixelConstBuffer());
	object::setModel(model);
}

GameObject::~GameObject()
{
	
}
static bool once = false;
void GameObject::draw(ID3D11DeviceContext*& immediateContext, bool sm)
{
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->PSSetConstantBuffers(0, 1, &this->getPixelConstBuffer());
	model->draw(immediateContext, sm);
	
}

void GameObject::drawDefTest(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->PSSetConstantBuffers(0, 1, &this->getPixelConstBuffer());
	model->drawDefTest(immediateContext);
}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex, bool pixel)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
	if (pixel) {
	}
}
