#pragma once
#include "Graphics.h"
#include "plHelper.h"

struct SPCB {
	struct {
		float element[4];
	}lightPos;
};

class ShadowMap {
public:
	ShadowMap(Light* light, Graphics* gfx);
	bool render(Graphics*& gfx,
		DirectX::XMMATRIX worldMatrix,
		DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix,
		DirectX::XMMATRIX lightViewMatrix,
		DirectX::XMMATRIX lightProjectionMatrix,
		ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* depthMapTexture);
private:
	//set up
	bool inputdesc(Graphics*& gfx, std::string& VbyteCode);
	bool sampler(Graphics*& gfx);
	bool buffers(Graphics*& gfx);
	//update 
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
	bool setShaderParameter(Graphics*& gfx, DirectX::XMMATRIX worldMatrix,
		DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix,
		DirectX::XMMATRIX lightViewMatrix,
		DirectX::XMMATRIX lightProjectionMatrix,
		ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* depthMapTexture);
	//object
	Light* light;

	ID3D11VertexShader* vertexShadow;
	ID3D11PixelShader* pixelShadow;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampleStateWrap;

	ID3D11SamplerState* sampleStateClamp;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* lightBuffer2;
	ID3D11InputLayout* inputLayout;
};