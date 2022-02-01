#pragma once
#include "Model.h"
#include "plHelper.h"

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
	ModelObj* get_Ball();
	ModelObj* get_Stol();
	ModelObj*get_IDK();
	//Mesh* get_starwars();
#pragma endregion
	
private:
#pragma region Textures
	ID3D11ShaderResourceView** def;//0 = difdef 1 = ndef
	ID3D11ShaderResourceView* Fire;
#pragma endregion

#pragma region Meshes
	//meshes
	ModelObj *ball;
	ModelObj *stol;
	ModelObj *IDK;
	//Mesh* starwars;
#pragma endregion

	void cantLoad(LPCWSTR theerror);
};