#pragma once
#include "Mesh.h"

class Graphics;

class ModelObj {
public:
	ModelObj(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def);
	ModelObj();
	void init(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def);
	void createVertexBuffeer(Graphics*& gfx, std::vector<vertex> vertecies);
	virtual ~ModelObj();
	void draw(Graphics*& gfx, bool shadowmap = false);
	std::vector<MeshObj> &getMehses();
	std::vector<Material*> &getMatrial();
	ID3D11Buffer*& getVertexBuffer();
	vec3* getBoundingBox();
private:
	bool loadModel(const std::string& ModelFile);
	ID3D11Buffer* vertexBuffer;
	std::vector<MeshObj> mMeshes;
	std::vector<Material*> matrial;
	vec3 boxSize[2];
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