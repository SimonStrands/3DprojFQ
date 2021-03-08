#pragma once
#include <fstream>
#include <sstream>
#include "otherHelps.h"
#include "Vec.h"
#include <DirectXMath.h>
//git

//class FileReader {
//public:
//	FileReader();
//	bool readObjFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes);
//
//private:
//	std::vector<vec3> calcTangent(vertex *vex1, vertex *vex2, vertex *vex3);
//	void fixtangent(std::vector<std::vector<vertex>>& objP);
//	std::vector<std::string> mtl;
//};

//material

bool readObjFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes);
std::vector<vec3> calcTangent(vertex* vex1, vertex* vex2, vertex* vex3);
void fixtangent(std::vector<std::vector<vertex>>& objP);
std::vector<std::string> getTextureNames(std::string fileName);