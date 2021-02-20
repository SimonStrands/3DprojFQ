#include "ReadObjFile.h"
//#thisNeedFix

FileReader::FileReader()
{

}


std::vector<vec3> FileReader::calcTangent(vertex *vex1, vertex *vex2, vertex *vex3)
{
	vertex vex[3]{ *vex1, *vex2, *vex3 };
	//get all vars we need
	DirectX::XMFLOAT3 e[2];
	DirectX::XMFLOAT2 deltaUV[2];
	vec3 v[3];
	vec2 uv[3];
#pragma region addVAndUV
	arrayToVec(vex[0].pos, v[0]);
	arrayToVec(vex[1].pos, v[1]);
	arrayToVec(vex[2].pos, v[2]);
	arrayToVec(vex[0].uv, uv[0]);
	arrayToVec(vex[1].uv, uv[1]);
	arrayToVec(vex[2].uv, uv[2]);
#pragma endregion addVAndUV

	vec3 dae = v[1] - v[0];
	e[0] = DirectX::XMFLOAT3(dae.x, dae.y, dae.z);
	dae = (v[2] - v[0]);
	e[1] = DirectX::XMFLOAT3(dae.x, dae.y, dae.z);
	//x = u    y = v
	deltaUV[0] = DirectX::XMFLOAT2(uv[1].x - uv[0].x, uv[1].y - uv[0].y);
	deltaUV[1] = DirectX::XMFLOAT2(uv[2].x - uv[0].x, uv[2].y - uv[0].y);
	DirectX::XMMATRIX a(//the delta
		deltaUV[1].y, -deltaUV[0].y, 0, 0,
		-deltaUV[1].x, deltaUV[0].x, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
	DirectX::XMMATRIX b(//the E:s
		e[0].x, e[0].y, e[0].z, 0,
		e[1].x, e[1].y, e[1].z, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
	DirectX::XMMATRIX res =
		1 / (deltaUV[0].x * deltaUV[1].y - deltaUV[0].y * deltaUV[1].x)
		* a * b;
	DirectX::XMFLOAT4X4 mtheReturn;
	DirectX::XMStoreFloat4x4(&mtheReturn, res);

	std::vector<vec3> theReturn;
	theReturn.push_back(vec3(mtheReturn._11, mtheReturn._12, mtheReturn._13));
	theReturn.push_back(vec3(mtheReturn._21, mtheReturn._22, mtheReturn._23));
	return theReturn;
}

void FileReader::readObjFile(std::vector<std::vector<vertex>>& objP, std::string fileName, int& nrOfVertexes)
{
	std::string* sTemp;
	std::string sTemp2[4];

	std::vector<std::array<float, 3>>vPos;
	std::vector<std::array<float, 2>>vUv;
	std::vector<std::array<float, 4>>vNorm;
	//(%2 = 0) = vertecies (%2 = 1) = uv 
	std::vector<int> whereinarray;

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
			if (sTemp2[3] != "") {
				for (int i = 0; i < 3; i++) {
					nrOfVertexes++;
					sTemp = getDest(sTemp2[i]);
					objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
					whereinarray.push_back(std::stoi(sTemp[0]) - 1);
					whereinarray.push_back(std::stoi(sTemp[1]) - 1);
					delete[] sTemp;
				}
				nrOfVertexes += 3;
				nrOfVertexes++;
				sTemp = getDest(sTemp2[3]);
				objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				whereinarray.push_back(std::stoi(sTemp[0]) - 1);
				whereinarray.push_back(std::stoi(sTemp[1]) - 1);
				delete[] sTemp;
				sTemp = getDest(sTemp2[2]);
				objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				whereinarray.push_back(std::stoi(sTemp[0]) - 1);
				whereinarray.push_back(std::stoi(sTemp[1]) - 1);
				delete[] sTemp;
				sTemp = getDest(sTemp2[0]);
				objP[objIndex].push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				whereinarray.push_back(std::stoi(sTemp[0]) - 1);
				whereinarray.push_back(std::stoi(sTemp[1]) - 1);
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
	fixtangent(objP);
}

void FileReader::fixtangent(std::vector<std::vector<vertex>>& objP)
{
	for (int objI = 0; objI < objP.size(); objI++) {
		for (int i = 0; i < objP[objI].size();) {
			std::vector<vec3> tanbi = calcTangent(&objP[objI][i], &objP[objI][i+1], &objP[objI][i+2]);
			for (int p = 0; p < 3; p++) {
				objP[objI][i + p].fixtang(tanbi[0], tanbi[1]);
			}
			i += 3;
		}
	}
}
