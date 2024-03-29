#pragma once
#include <fstream>
#include <sstream>
#include "otherHelps.h"
#include "Vec.h"
#include "Material.h"
#include "Model.h"
//git

class ResourceManager;
//material
void getLowest(vec3 box[2], std::array<float, 3> vPos);
bool readObjFile(std::vector<MeshObj>& Meshes, std::string fileName, std::vector<Material*> &matrial, Graphics*& gfx, vec3 box[2]);
std::vector<vec3> calcTangent(vertex* vex1, vertex* vex2, vertex* vex3);
void fixtangent(std::vector<vertex> &vertecies, std::vector<DWORD>& indecies);
bool getMatrialFromFile(std::string fileName, std::vector<Material*> &matrial, Graphics*& gfx, ResourceManager* rm);