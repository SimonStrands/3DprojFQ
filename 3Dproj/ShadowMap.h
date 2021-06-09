#pragma once
#include "plHelper.h"
#include "Light.h"
#include <d3d11.h>
#include "plHelper.h"
class Graphics;

class ShadowMap {
public:
	ShadowMap(Light* light, Graphics* gfx);
	ID3D11DepthStencilView* Getdepthview();
	ID3D11ShaderResourceView* GetshadowResV();
	void RenderShader();
	ID3D11ShaderResourceView*& fromDepthToSRV();
	
private:

	bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height);
	//object
	Light* light;
	Graphics* gfx;
	
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	ID3D11VertexShader* vertexShadow;
	ID3D11PixelShader* pixelShadow;
	ID3D11Resource* shadowRes;
	ID3D11ShaderResourceView* shadowResV;

	ID3D11RenderTargetView* shadowTarget;
	/*set up
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
	*/
};