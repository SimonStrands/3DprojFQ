#include "GameObject.h"

GameObject::GameObject(ModelObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	this->model = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	object::setModel(model);
	drawed = false;
}

GameObject::~GameObject()
{
	
}
static bool once = false;
void GameObject::draw(Graphics *&gfx, bool sm)
{
	drawed = true;
	gfx->get_IC()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IC()->DSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IC()->HSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->draw(gfx, sm);
}

void GameObject::drawDebug(Graphics*& gfx, bool sm)
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
	
		model->getMehses()[i].getMatrial()->flags.Maps[4] = tess;
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


void GameObject::getBox(DirectX::XMVECTOR theReturn[])
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	DirectX::XMVECTOR low =  { model->getBox()[1].x ,model->getBox()[1].y ,model->getBox()[1].z , 1 };
	DirectX::XMVECTOR high = { model->getBox()[0].x ,model->getBox()[0].y ,model->getBox()[0].z , 1 };

	theReturn[0] = DirectX::XMVectorAdd(DirectX::XMVector4Transform(DirectX::XMVectorMultiply(low, this->getScale().toXMvector()), rot), this->getPos().toXMvector());
	theReturn[1] = DirectX::XMVectorAdd(DirectX::XMVector4Transform(DirectX::XMVectorMultiply(high, this->getScale().toXMvector()), rot), this->getPos().toXMvector());
	DirectX::XMVECTOR temp = {fmax(theReturn[1].m128_f32[0],theReturn[0].m128_f32[0]),fmax(theReturn[1].m128_f32[1],theReturn[0].m128_f32[1]),fmax(theReturn[1].m128_f32[2],theReturn[0].m128_f32[2]) };
	DirectX::XMVECTOR temp2 = { std::min(theReturn[1].m128_f32[0],theReturn[0].m128_f32[0]),std::min(theReturn[1].m128_f32[1],theReturn[0].m128_f32[1]),std::min(theReturn[1].m128_f32[2],theReturn[0].m128_f32[2]) };
	theReturn[0] = temp2;
	theReturn[1] = temp;
}

const bool GameObject::isDrawed()
{
	return this->drawed;
}

void GameObject::clearDrawed()
{
	this->drawed = false;
}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex, bool pixel)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
	if (pixel) {
	}
}
