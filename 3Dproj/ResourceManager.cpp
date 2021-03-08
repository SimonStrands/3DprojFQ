#include "ResourceManager.h"

ResourceManager::ResourceManager(Graphics*& gfx)
{
	loadThings(gfx);
}

ResourceManager::~ResourceManager()
{
	if (def != nullptr) {
		def->Release();
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
	if (!CreateTexture("Textures/Default/KdDef.png", gfx->getDevice(), gfx->getTexture(), def)) {
		cantLoad(L"kddef cant load");
	}
	if (!CreateTexture("Textures/Fire.png", gfx->getDevice(), gfx->getTexture(), Fire)) {
		cantLoad(L"Fire cant load");
	}
	//mesh
	ball = new Mesh(gfx, "obj/newsun.obj", def);
	stol = new Mesh(gfx, "obj/stol.obj", def);
	IDK = new Mesh(gfx, "obj/untitled.obj", def);
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

ID3D11ShaderResourceView* ResourceManager::getDef()
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
