#include "ReadObjFile.h"
//#thisNeedFix
//get to use assimp
FileReader::FileReader()
{

}
//
//std::string FileReader::getmtl()
//{
//	return mtl[0];
//}

void FileReader::readObjFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes)
{
	std::string* sTemp;
	std::string sTemp2[4];

	std::vector<std::array<float, 3>>vPos;
	std::vector<std::array<float, 2>>vUv;
	std::vector<std::array<float, 4>>vNorm;

	std::ifstream infile(fileName);
	std::string readWord;
	std::string trash;
	int objIndex = -1;

	while (std::getline(infile, readWord)) {
		//get all data
		if (readWord.substr(0, 2) == "v ") {
			std::istringstream a;
			vPos.resize(vPos.size() + 1);
			a.str(readWord);
			a >> trash >> vPos[vPos.size() - 1][0] >> vPos[vPos.size() - 1][1] >> vPos[vPos.size() - 1][2];
		}
		else if (readWord.substr(0, 3) == "vt ") {
			std::istringstream a;
			a.str(readWord);
			vUv.resize(vUv.size() + 1);
			a >> trash >> vUv[vUv.size() - 1][0] >> vUv[vUv.size() - 1][1];
		}
		else if (readWord.substr(0, 3) == "vn ") {
			std::istringstream a;
			a.str(readWord);
			vNorm.resize(vNorm.size() + 1);
			a >> trash >> vNorm[vNorm.size() - 1][0] >> vNorm[vNorm.size() - 1][1] >> vNorm[vNorm.size() - 1][2];
			vNorm[vNorm.size() - 1][3] = 0;
		}
		//make data to vertex
		//ask if this is ok
		else if (readWord.substr(0, 2) == "f ") {
			std::istringstream a;
			a.str(readWord);
			a >> trash >> sTemp2[0] >> sTemp2[1] >> sTemp2[2] >> sTemp2[3];
			if(sTemp2[3] != ""){
				for (int i = 0; i < 3; i++) {
					nrOfVertexes++;
					sTemp = getDest(sTemp2[i]);
					objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
					delete[] sTemp;
				}
				nrOfVertexes += 3;
				sTemp = getDest(sTemp2[3]);
				objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
				sTemp = getDest(sTemp2[2]);
				objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
				sTemp = getDest(sTemp2[0]);
				objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
			}
			else {
				for (int i = 0; i < 3; i++) {
					nrOfVertexes++;
					sTemp = getDest(sTemp2[i]);
					objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
					delete[] sTemp;
				}
			}
		}
		else if (readWord.substr(0, 6) == "mtllib") 
		{
			std::istringstream a;
			a.str(readWord);
			std::string mtlname;
			a >> trash >> mtlname;
			mtl.push_back(mtlname);
		}
		else if (readWord.substr(0, 2) == "o ") {
			objIndex++;
			objP.resize(objP.size() + 1);
		}
	}
}

//void FileReader::readMtlFile()
//{
//	
//}
