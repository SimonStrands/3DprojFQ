#include "ResourceManager.h"

ResourceManager::ResourceManager(Graphics*& gfx)
{
	loadThings(gfx);
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < 4; i++) {
		if (def[i] != nullptr) {
			def[i]->Release();
		}
	}
	delete[] def;
	
	if (Fire != nullptr) {
		Fire->Release();
	}
	
	std::map<std::string, ModelObj*>::iterator it;
	for (it = Models.begin(); it != Models.end(); it++) {
		delete it->second;
	}
	
	delete ball;
	delete stol;
	delete IDK;
}

void loadWithThread(threadInfo thredData)
{
	thredData.model->init(thredData.name, thredData.gfx, thredData.def);
}

void ResourceManager::loadThings(Graphics*& gfx)
{
	//default tex
	def = new ID3D11ShaderResourceView * [4];
	//diffuse
	if (!CreateTexture("Textures/Default/KdDef.png", gfx->getDevice(), gfx->getTexture(), def[0])) {
		cantLoad(L"kddef cant load");
	}
	//normal
	if (!CreateTexture("Textures/Default/NormalDef2.png", gfx->getDevice(), gfx->getTexture(), def[1])) {
		cantLoad(L"NDef cant load");
	}
	//ambient
	if (!CreateTexture("Textures/Default/KaDef2.png", gfx->getDevice(), gfx->getTexture(), def[2])) {
		cantLoad(L"AmientDef cant load");
	}
	//specular//orkar inte
	def[3] = def[2];
	
	if (!CreateTexture("Textures/Fire.png", gfx->getDevice(), gfx->getTexture(), Fire)) {
		cantLoad(L"Fire cant load");
	}
	def[3] = nullptr;
	//mesh
	
	std::vector<std::thread> thrs;

	ball = new ModelObj("obj/GroundLowPloy.obj", gfx, def);
	
	stol = new ModelObj("obj/DoubleMesh.obj" , gfx, def);
	
	IDK = new  ModelObj("obj/quad2.obj"     , gfx, def);
	std::string names[] = {
		"roundsol.obj",
		"stormtrooper.obj",
		"DCube.obj",
		"stol.obj",
		"Sting-Sword-lowpoly.obj",
		"indoor_plant_02.obj",
		"Camera.obj"
	};
	for (int i = 0; i < _countof(names); i++) {
		ModelObj* model = new ModelObj();
		//threadInfo threadParam({model, "obj/" + names[i], def, gfx});
		//thrs.push_back(std::thread(loadWithThread, std::ref(model), "obj/" + names[i], std::ref(def), std::ref(gfx)));
		//thrs.push_back(std::thread(loadWithThread, std::ref(threadParam)));
		model->init("obj/" + names[i], gfx, def);
		Models.insert(std::make_pair(names[i], model));
	}
	//for (int i = 0; i < _countof(names); i++) {
	//	thrs[i].join();
	//}
	for (int i = 0; i < _countof(names); i++) {
		int sizeOfMatrials = Models.find(names[i])->second->getMatrial().size();
		ModelObj* model = Models.find(names[i])->second;
		for (int p = 0; p < sizeOfMatrials; p++) {
			TC::GetInst().add(model->getMatrial()[p]);
		}
	}
	std::cout << "resource manager is done loading" << std::endl;
}

ModelObj* ResourceManager::get_Models(std::string key)
{
	return Models.find(key)->second;
}

ModelObj*ResourceManager::get_Ball()
{
	return ball;
}

ModelObj*ResourceManager::get_Stol()
{
	return stol;
}

ModelObj*ResourceManager::get_IDK()
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
