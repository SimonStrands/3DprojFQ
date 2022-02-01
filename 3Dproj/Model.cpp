#include "Model.h"
#include "ReadObjFile.h"
/*
Model::Model(const std::string filePath, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale)
{
	this->changePos(pos);
	this->changeScale(scale);
	this->changeRot(rot);
	this->gfx = gfx;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreatePixelConstBuffer(gfx, this->getPixelConstBuffer());
	if (filePath != "") {
		LoadModel(filePath);
	}
	
}

Model::Model(Graphics*& gfx)
{
	this->gfx = gfx;
}

void Model::draw(ID3D11DeviceContext*& immediateContext)
{
	for (int i = 0; i < mMeshes.size(); i++) {
		std::cout << mMeshes[i].nrOfIndecies << std::endl;
	}
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->PSSetConstantBuffers(0, 1, &this->getPixelConstBuffer());
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].Draw(immediateContext);
	}
}


void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(this->ProcessMesh(mesh, scene));
	}
	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<vertex> vertecies;
	std::vector<DWORD> indices;
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		vertex vert;
		vert.pos[0] = mesh->mVertices[i].x;
		vert.pos[1] = mesh->mVertices[i].y;
		vert.pos[2] = mesh->mVertices[i].z;
		vert.norm[0] = mesh->mNormals[i].x;
		vert.norm[1] = mesh->mNormals[i].y;
		vert.norm[2] = mesh->mNormals[i].z;

		std::cout << vert.pos[0] <<","<< vert.pos[1]<<"," << vert.pos[2] << "\n";
		if (mesh->mTextureCoords[0]) {
			vert.uv[0] = mesh->mTextureCoords[0][i].x;
			vert.uv[1] = mesh->mTextureCoords[0][i].y;
		}
		vertecies.push_back(vert);
	}
	for (UINT i = 0; i < (int)mesh->mNumFaces; i++) 
	{
		aiFace face = mesh->mFaces[i];

		for (UINT o = 0; o < face.mNumIndices; o++) 
		{
			indices.push_back(face.mIndices[o]);
		}
	}
	
	return Mesh(gfx, vertecies, indices);
}

bool Model::LoadModel(const std::string& modelfile)
{
	Assimp::Importer importer;


	const aiScene* pScene = importer.ReadFile(modelfile,
		aiProcess_Triangulate
		| aiProcess_ConvertToLeftHanded
		| aiProcess_GenNormals
		| aiProcess_JoinIdenticalVertices
	);

	if (pScene == NULL) {
		return false;
	}
	this->ProcessNode(pScene->mRootNode, pScene);
	return false;
}

*/

ModelObj::ModelObj(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	std::vector<FileTextureData> textures;
	std::vector<Material> matrial;
	textures = getTextureNames(ModelFile);
	matrial.resize(textures.size());
	for (int i = 0; i < textures.size(); i++) {
		textures[i].fromFTDToMaterial(matrial[i], gfx, def);
	}
	readObjFile(mMeshes, ModelFile, matrial, gfx);
}

void ModelObj::draw(ID3D11DeviceContext*& immediateContext, bool sm)
{
	for (int i = 0; i < mMeshes.size(); i++) {
		if (!sm) {
			this->mMeshes[i].SetShader(immediateContext);
		}
		mMeshes[i].draw(immediateContext);
	}
}

void ModelObj::drawDefTest(ID3D11DeviceContext*& immediateContext)
{
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].draw(immediateContext);
	}
}

std::vector<MeshObj> ModelObj::getMehses()
{
	return this->mMeshes;
}

bool ModelObj::loadModel(const std::string& ModelFile)
{
	return false;
}