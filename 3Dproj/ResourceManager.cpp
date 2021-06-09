#include "ResourceManager.h"

ResourceManager::ResourceManager(Graphics*& gfx)
{
	loadThings(gfx);
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < 2; i++) {
		if (def[i] != nullptr) {
			def[i]->Release();
		}
	}
	if (Fire != nullptr) {
		Fire->Release();
	}
	delete ball;
	delete stol;
	delete IDK;
}

void ResourceManager::loadThings(Graphics*& gfx)
{
	//default tex
	def = new ID3D11ShaderResourceView * [2];
	if (!CreateTexture("Textures/Default/KdDef.png", gfx->getDevice(), gfx->getTexture(), def[0])) {
		cantLoad(L"kddef cant load");
	}
	if (!CreateTexture("Textures/Default/normalDef.jpg", gfx->getDevice(), gfx->getTexture(), def[1])) {
		cantLoad(L"NDef cant load");
	}
	if (!CreateTexture("Textures/Fire.png", gfx->getDevice(), gfx->getTexture(), Fire)) {
		cantLoad(L"Fire cant load");
	}
	//mesh
	ball = new Mesh(gfx, "obj/newsun.obj", def);
	stol = new Mesh(gfx, "obj/stol.obj", def);
	IDK = new Mesh(gfx, "obj/quad.obj", def);
	//starwars = new Mesh(gfx, "obj/stormtrooper.obj", def);
}

Mesh *ResourceManager::get_Ball()
{
	return ball;
}

Mesh *ResourceManager::get_Stol()
{
	return stol;
}

Mesh *ResourceManager::get_IDK()
{
	return IDK;
}

//Mesh* ResourceManager::get_starwars()
//{
//	return this->starwars;
//}

ID3D11ShaderResourceView** ResourceManager::getDef()
{
	return this->def;
}

ID3D11ShaderResourceView* ResourceManager::getFire()
{
	return this->Fire;
}

void ResourceManager::cantLoad(LPCWSTR theerror)
{
	MessageBox(nullptr, theerror, L"ERROR", MB_ICONWARNING | MB_OK);
}
