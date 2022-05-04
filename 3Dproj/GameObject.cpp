#include "GameObject.h"

GameObject::GameObject(ModelObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->setPos(pos);
	this->setScale(scale);
	this->setRot(rot);
	this->model = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	object::setModel(model);
	drawed = false;
}

GameObject::~GameObject()
{
	
}

void GameObject::draw(Graphics *&gfx, bool sm)
{
	drawed = true;
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IMctx()->DSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IMctx()->HSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->draw(gfx, sm);
}


void GameObject::setTesselation(bool tess, Graphics*& gfx)
{
	for (int i = 0; i < model->getMehses().size(); i++) {
	
		model->getMehses()[i].setTesselation(tess, gfx);
	
	}
}


void GameObject::getBoundingBox(DirectX::XMFLOAT4 theReturn[])
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));
	
	vec3 high(model->getBoundingBox()[1].x, model->getBoundingBox()[1].y, model->getBoundingBox()[1].z);
	vec3 low(model->getBoundingBox()[0].x, model->getBoundingBox()[0].y, model->getBoundingBox()[0].z);
	
	DirectX::XMMATRIX modelMatrix = getWorldMatrix();
	
	DirectX::XMVECTOR bbPoints[8] = {
		{high.x,high.y,high.z,1},
		{high.x,high.y,low.z,1},
		{high.x,low.y,high.z,1},
		{high.x,low.y,low.z,1},
		{low.x,high.y,high.z,1},
		{low.x,high.y,low.z,1},
		{low.x,low.y,high.z,1},
		{low.x,low.y,low.z,1}
	};
	
	bbPoints[0] = DirectX::XMVector4Transform(bbPoints[0], modelMatrix);
	DirectX::XMStoreFloat4(&theReturn[0], bbPoints[0]);
	DirectX::XMStoreFloat4(&theReturn[1], bbPoints[0]);
	
	for (int i = 1; i < 8; i++) {
		bbPoints[i] = DirectX::XMVector4Transform(bbPoints[i], modelMatrix);
		DirectX::XMFLOAT4 tester;
		DirectX::XMStoreFloat4(&tester, bbPoints[i]);

		if (tester.x > theReturn[1].x) {
			theReturn[1].x = tester.x;
		}
		else if (tester.x < theReturn[0].x) {
			theReturn[0].x = tester.x;
		}
		if (tester.y > theReturn[1].y) {
			theReturn[1].y = tester.y;
		}
		else if (tester.y < theReturn[0].y) {
			theReturn[0].y = tester.y;
		}
		if (tester.z > theReturn[1].z) {
			theReturn[1].z = tester.z;
		}
		else if (tester.z < theReturn[0].z) {
			theReturn[0].z = tester.z;
		}
	}
}

const bool GameObject::isDrawed()
{
	return this->drawed;
}

void GameObject::clearDrawed()
{
	this->drawed = false;
}

void GameObject::Updateshaders(Graphics*& gfx, bool vertex)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}
}
