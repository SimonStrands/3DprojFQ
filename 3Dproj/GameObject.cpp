#include "GameObject.h"

GameObject::GameObject(ModelObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	this->model = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	object::setModel(model);
}

GameObject::~GameObject()
{
	
}
static bool once = false;
void GameObject::draw(Graphics *&gfx, bool sm)
{
	gfx->get_IC()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IC()->DSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IC()->HSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->draw(gfx, sm);
}

void GameObject::drawDefTest(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->drawDefTest(immediateContext);
}

void GameObject::setTesselation(bool tess, Graphics*& gfx)
{
	for (int i = 0; i < model->getMehses().size(); i++) {
	
		model->getMehses()[i].getMatrial().flags.Maps[4] = tess;
		if (tess) {
			model->getMehses()[i].SetShaders(gfx->getVS()[2]);
			model->getMehses()[i].SetShaders(gfx->getHS()[1], gfx->getDS()[1]);
		}
		else {
			model->getMehses()[i].SetShaders(gfx->getVS()[0]);
			model->getMehses()[i].SetShaders((ID3D11HullShader*)nullptr, nullptr);
		}
	
	}
}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex, bool pixel)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
	if (pixel) {
	}
}
