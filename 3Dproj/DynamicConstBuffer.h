/*#pragma once
#include "Graphics.h"
enum varType { STRING, FLOATY, FLOAT2, FLOAT3, FLOAT4, FLOAT3X3, FLOAT4X4 };

template<typename Y, typename X>
struct V2A {
	Y y;
	X x;
};

class CreateCBDynamic {
public:
	void add(std::string value, std::string name);
	void add(float value, std::string name);
	void add(float value[2], std::string name);
	void add(float value[3], std::string name);
	void add(float value[4], std::string name);
	void add(float value[3][3], std::string name);
	void add(float value[4][4], std::string name);
	void Create(Graphics*& gfx, ID3D11Buffer*& buff);
private:
	std::vector<V2A<std::string, std::string>> vString;
	std::vector<V2A<std::string, float>> vFloat;
	std::vector<V2A<std::string, float[2]>> vFloat2;
	std::vector<V2A<std::string, float[3]>> vFloat3;
	std::vector<V2A<std::string, float[4]>> vFloat4;
	std::vector<V2A<std::string, float[3][3]>> vFloat3X3;
	std::vector<V2A<std::string, float[4][4]>> vFloat4X4;
};*/



