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
	//std::string getmtl();
	void readObjFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes);
	//void readMtlFile();
private:
	std::vector<std::string> mtl;
};

//material