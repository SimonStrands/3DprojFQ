#include "ReadObjFile.h"
#include <DirectXMath.h>


std::vector<vec3> calcTangent(vertex* vex1, vertex* vex2, vertex* vex3)
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
	float div = (deltaUV[0].x * deltaUV[1].y - deltaUV[0].y * deltaUV[1].x);
	if (div == 0) {
		div = 0.0001f;
	}
	DirectX::XMMATRIX res =
		1 / div
		* a * b;
	DirectX::XMFLOAT4X4 mtheReturn;
	DirectX::XMStoreFloat4x4(&mtheReturn, res);

	std::vector<vec3> theReturn;
	theReturn.push_back(vec3(mtheReturn._11, mtheReturn._12, mtheReturn._13));
	theReturn.push_back(vec3(mtheReturn._21, mtheReturn._22, mtheReturn._23));
	return theReturn;
}


void fixtangent(std::vector<vertex> &vertecies)
{
	for (int i = 0; i < vertecies.size();) {
		std::vector<vec3> tanbi = calcTangent(&vertecies[i], &vertecies[i+1], &vertecies[i+2]);
		tanbi[0].Normalize(); tanbi[1].Normalize();
		for (int p = 0; p < 3; p++) {
			vertecies[i + p].fixtang(tanbi[0], tanbi[1]);
		}
		i += 3;
	}
}

std::vector<FileTextureData> getTextureNames(std::string fileName)
{
	std::ifstream infile(fileName);
	std::string readWord;
	std::string trash;
	std::string mtlname;
	std::string TextureName;
	int CTR = -1;
	std::vector<FileTextureData> theReturn;
	if (!infile.is_open()) {
		printf("cannot open textures");
		return std::vector<FileTextureData>();
	}
	bool done = false;
	while (std::getline(infile, readWord) && !done) {
		if (readWord.substr(0, 6) == "mtllib") {
			std::istringstream a;
			a.str(readWord);
			a >> trash >> mtlname;
			done = true;
		}
	}
	infile.close();
	infile.open("obj/" + mtlname);
	if (done && infile.is_open()) {
		while (std::getline(infile, readWord)) {
			if (readWord.substr(0, 6) == "newmtl") {
				CTR++;
				theReturn.resize(CTR + 1);
				std::istringstream a;
				std::string b;
				a.str(readWord);
				a >> trash >> theReturn[CTR].name;
			}
			if (readWord.substr(0, 2) == "Ns") {
				//do something
				std::istringstream a;
				std::string b;
				a.str(readWord);
				a >> trash >> theReturn[CTR].Ns;
			}
			if (readWord.substr(0, 2) == "Ka") {
				//ambient
				std::istringstream a;
				a.str(readWord);
				a >> trash >> theReturn[CTR].Ka[0] >> theReturn[CTR].Ka[1] >> theReturn[CTR].Ka[2];
			}
			if (readWord.substr(0, 2) == "Kd") {
				//deffuse
				std::istringstream a;
				a.str(readWord);
				a >> trash >> theReturn[CTR].Kd[0] >> theReturn[CTR].Kd[1] >> theReturn[CTR].Kd[2];
			}
			if (readWord.substr(0, 6) == "map_Ka") {
				//map_ambient
				std::istringstream a;
				a.str(readWord);
				a >> trash >> theReturn[CTR].Map_Ka;
				theReturn[CTR].nrOfTextures++;
			}
			if (readWord.substr(0, 6) == "map_Kd") {
				//map_diffuse
				std::istringstream a;
				a.str(readWord);
				a >> trash >> theReturn[CTR].Map_Kd;
				theReturn[CTR].nrOfTextures++;
			}
			if (readWord.substr(0, 6) == "map_Ks") {
				//map_Specular
				std::istringstream a;
				a.str(readWord);
				a >> trash >> theReturn[CTR].Map_Ks;
				theReturn[CTR].nrOfTextures++;
			}
			if (readWord.substr(0, 8) == "map_Bump") {
				//map_normal
				std::istringstream a;
				a.str(readWord);
				std::string b;
				bool done = false;
				while (a >> b && !done) {
					if (!(b.substr(0, 2) == "-bm" || b.substr(0, 8) == "map_Bump" || int(b[0]) < 58)) {
						done = true;
						theReturn[CTR].Map_Bump = b;
						theReturn[CTR].nrOfTextures++;
					}
				}
			}
		}
	}
	else {
		//we didnt get what we wanted
		return std::vector<FileTextureData>();
	}

	return theReturn;
}

bool readObjFile(std::vector<MeshObj>& Meshes, std::string fileName, std::vector<Material> matrial, Graphics *& gfx)
{
	std::string* sTemp;
	std::string sTemp2[4];

	std::vector<vertex> vertecies;
	std::vector<std::array<float, 3>>vPos;
	std::vector<std::array<float, 2>>vUv;
	std::vector<std::array<float, 4>>vNorm;
	int currentMatrial = -1;

	std::ifstream infile(fileName);
	std::string readWord;
	std::string trash;
	bool first = true;
	if (!infile.is_open()) {
		return false;
	}

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
					//nrOfVertexes++;
					sTemp = getDest(sTemp2[i]);
					//när jag läser in faces så får dem första sex alltid samma p.g.a det är så dem har skrivit det på obj filen
					vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
					delete[] sTemp;
				}
				//nrOfVertexes += 3;
				sTemp = getDest(sTemp2[0]);
				vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
				sTemp = getDest(sTemp2[2]);
				vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
				sTemp = getDest(sTemp2[3]);
				vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
				delete[] sTemp;
			}
			else {
				for (int i = 0; i < 3; i++) {
					//nrOfVertexes++;
					sTemp = getDest(sTemp2[i]);
					vertecies.push_back(vertex(vPos[std::stoi(sTemp[0]) - 1], vUv[std::stoi(sTemp[1]) - 1], vNorm[std::stoi(sTemp[2]) - 1]));
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
		}
		else if (readWord.substr(0, 2) == "o ") {
			if (!first) {
				fixtangent(vertecies);
				Meshes.push_back(MeshObj(gfx, vertecies, matrial[currentMatrial]));
				if (matrial[currentMatrial].flags & TexFlags::Map_Bump) {
					Meshes[Meshes.size() - 1].SetShaders(gfx->getVS()[0], gfx->getPS()[0]);
				}
				else {
					Meshes[Meshes.size() - 1].SetShaders(gfx->getVS()[0], gfx->getPS()[2]);
				}
				vertecies.clear();
			}
			first = false;
		}
		else if (readWord.substr(0, 6) == "usemtl") {
			std::istringstream a;
			a.str(readWord);
			std::string mName;
			a >> trash >> mName;
			for (int i = 0; i < matrial.size(); i++) {
				if (matrial[i].name == mName) {
					currentMatrial = i;
				}
			}
		}
	}
	fixtangent(vertecies);
	Meshes.push_back(MeshObj(gfx, vertecies, matrial[currentMatrial]));
	if (matrial[currentMatrial].flags & TexFlags::Map_Bump) {
		Meshes[Meshes.size() - 1].SetShaders(gfx->getVS()[0], gfx->getPS()[0]);
	}
	else {
		Meshes[Meshes.size() - 1].SetShaders(gfx->getVS()[0], gfx->getPS()[2]);
	}
	
	return true;
}

