#pragma once
#include <fstream>
#include <sstream>
#include "otherHelps.h"
#include "Vec.h"
//git
struct mtl {
	vec3 ka;
	vec3 kd;
	vec3 ks;

};
class FileReader {
public:
	FileReader();
	void readObjFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes);
	void fixtangent(std::vector<std::vector<vertex>>& objP, std::vector<int>& whereinarray, std::vector<std::array<float, 3>> &vPos, std::vector <std::array<float, 2>> &uv);
	vec4 calcTangent(std::array<float, 3> va[3], std::array<float, 2> uva[3]);
private:
	std::vector<std::string> mtl;
};

//material