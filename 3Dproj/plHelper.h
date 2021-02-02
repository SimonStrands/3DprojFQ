#pragma once
#include <d3d11.h>
#include "vertex.h"
//git
bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vBuffer, ID3D11VertexShader*& vShader, 
	ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,
	ID3D11Texture2D*& tex, ID3D11ShaderResourceView*& textureRSV, ID3D11SamplerState*& sampler);