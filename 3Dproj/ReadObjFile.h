#pragma once
#include <fstream>
#include <sstream>
#include "otherHelps.h"
//git
class FileReader {
public:
	FileReader();
	//std::string getmtl();
	void readObjFile(std::vector<vertex>& objP, std::string fileName, int& nrOfTriangles);
	//void readMtlFile();
private:
	std::vector<std::string> mtl;
};

//material