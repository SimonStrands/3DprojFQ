#pragma once
#include "Mesh.h"
#include "plHelper.h"

class ResourceManager {
public:
	ResourceManager(Graphics *& gfx);
	~ResourceManager();
	
	void loadThings(Graphics*& gfx);
#pragma region Textures
	ID3D11ShaderResourceView* getDef();
	ID3D11ShaderResourceView* getFire();
#pragma endregion

#pragma region Meshes
	Mesh *get_Ball();
	Mesh *get_Stol();
	Mesh *get_IDK();
#pragma endregion
	
private:
#pragma region Textures
	ID3D11ShaderResourceView* def;
	ID3D11ShaderResourceView* Fire;
#pragma endregion

#pragma region Meshes
	//meshes
	Mesh *ball;
	Mesh *stol;
	Mesh *IDK;
#pragma endregion

	void cantLoad(LPCWSTR theerror);
};