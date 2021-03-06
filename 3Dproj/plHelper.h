#pragma once
#include <d3d11.h>
#include "vertex.h"
#include <string>
//git
bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader**& vShader,
	ID3D11PixelShader**& pShader, ID3D11GeometryShader**& gShader,
	ID3D11InputLayout**& inputLayout,
	ID3D11Texture2D*& tex, ID3D11SamplerState*& sampler);


bool loadVShader(std::string name, ID3D11Device* device, ID3D11VertexShader*& vShader, std::string& vShaderByteCode);
bool loadGShader(std::string name, ID3D11Device* device, ID3D11GeometryShader*& gShader);
bool loadPShader(std::string name, ID3D11Device* device, ID3D11PixelShader*& pShader);
bool CreateTexture(std::string file, ID3D11Device* device, ID3D11Texture2D*& tex, ID3D11ShaderResourceView*& texSRV);