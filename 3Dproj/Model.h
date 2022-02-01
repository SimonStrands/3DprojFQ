#pragma once
#include "Mesh.h"

class Graphics;

class ModelObj {
public:
	ModelObj(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def);
	void draw(ID3D11DeviceContext*& immediateContext, bool sm = false);
	void drawDefTest(ID3D11DeviceContext*& immediateContext);
	std::vector<MeshObj> getMehses();
private:
	bool loadModel(const std::string& ModelFile);
	std::vector<MeshObj> mMeshes;
};

/*
class Model
{
public:
	//loads model and create VCB and PCB
	Model(const std::string filePath, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	//most basic things
	Model(Graphics*& gfx);
	bool LoadModel(const std::string& modelfile);
	void draw(ID3D11DeviceContext*& immediateContext);
protected:
	std::vector<Mesh> mMeshes;
	Graphics* gfx;
private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	
};
*/