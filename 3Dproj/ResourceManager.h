#pragma once
#include "Model.h"
#include "plHelper.h"
#include <map>
#include <thread>
#include "TrashCollector.h"

struct threadInfo {
	ModelObj* model;
	std::string name;
	ID3D11ShaderResourceView** def;
	Graphics*& gfx;
};

class ResourceManager {
public:
	ResourceManager(Graphics *& gfx);
	~ResourceManager();
	
	void loadThings(Graphics*& gfx);
#pragma region Textures
	ID3D11ShaderResourceView** getDef();
	ID3D11ShaderResourceView* getFire();
#pragma endregion

#pragma region Meshes
	ModelObj* get_Models(std::string key);
	ModelObj* get_Ball();
	ModelObj* get_Stol();
	ModelObj* get_IDK();
	//Mesh* get_starwars();
#pragma endregion
	
private:
#pragma region Textures
	ID3D11ShaderResourceView** def;//0 = difdef 1 = ndef
	ID3D11ShaderResourceView* Fire;
#pragma endregion

#pragma region Meshes
	//meshes
	std::map<std::string, ModelObj*> Models;
	ModelObj *ball;
	ModelObj *stol;
	ModelObj *IDK;
	//Mesh* starwars;
#pragma endregion

	void addMaterialToTrashCollector(ModelObj* model);
	void cantLoad(LPCWSTR theerror);
};