#pragma once
#include <d3d11.h>
#include <vector>
//#include "Material.h"
//trash collector
class TC {
public:
	static TC& GetInst();
	void empty();
	//void add(Material& matrial);
	void add(ID3D11ShaderResourceView* SRV);
private:
	TC() {};
	static TC s_Instance;
	//std::vector<Material&> Matrial;
	std::vector<ID3D11ShaderResourceView*> shaderRSV;
	
};