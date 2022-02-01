#pragma once
#include <fstream>
#include <sstream>
#include "otherHelps.h"
#include "Vec.h"
#include "Material.h"
#include "Model.h"
//git

//material
bool readObjFile(std::vector<MeshObj>& Meshes, std::string fileName, std::vector<Material> matrial, Graphics*& gfx);
std::vector<vec3> calcTangent(vertex* vex1, vertex* vex2, vertex* vex3);
void fixtangent(std::vector<MeshObj> meshes);
std::vector<FileTextureData> getTextureNames(std::string fileName);
//bool readFBXFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes);

