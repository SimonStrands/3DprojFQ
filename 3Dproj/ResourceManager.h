#pragma once
#include "Mesh.h"
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
	MeshObj *get_Ball();
	MeshObj *get_Stol();
	MeshObj *get_IDK();
	//Mesh* get_starwars();
#pragma endregion
	
private:
#pragma region Textures
	ID3D11ShaderResourceView** def;//0 = difdef 1 = ndef
	ID3D11ShaderResourceView* Fire;
#pragma endregion

#pragma region Meshes
	//meshes
	MeshObj *ball;
	MeshObj *stol;
	MeshObj *IDK;
	//Mesh* starwars;
#pragma endregion

	void cantLoad(LPCWSTR theerror);
};