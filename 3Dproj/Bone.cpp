#include "Bone.h"
/*
Bone::Bone(int id, std::string name, DirectX::XMMATRIX bindLocalTransform)
{
	this->id = id;
	this->name = name;
	this->localTransform = bindLocalTransform;
}

void Bone::addChild(Bone cBone)
{
	children.push_back(cBone);
}

void Bone::setAnimationTransform(DirectX::XMMATRIX animT)
{
	this->animatonTransform = animT;
}

DirectX::XMMATRIX Bone::getAnimTransform()
{
	return this->animatonTransform;
}

void Bone::calcInverseBindTransform(DirectX::XMMATRIX ParentBindTransform)
{
	DirectX::XMMATRIX bindTransform = ParentBindTransform * localTransform;
	this->inverseTransform = DirectX::XMMatrixInverse(nullptr, bindTransform);
	for (int i = 0; i < children.size(); i++) {
		children[i].calcInverseBindTransform(bindTransform);
	}

}

DirectX::XMMATRIX Bone::getInverseTransform()
{
	return this->inverseTransform;
}
*/