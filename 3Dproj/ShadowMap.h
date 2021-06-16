#pragma once
#include "plHelper.h"
#include "Light.h"
#include <d3d11.h>
#include "plHelper.h"
#include <DirectXMath.h>
#include "CreateBuffer.h"

class Graphics;

class ShadowMap {
public:
	ShadowMap(PointLight* light, Graphics* gfx);
	virtual ~ShadowMap();
	ID3D11DepthStencilView* Getdepthview();
	ID3D11ShaderResourceView*& GetshadowResV();
	ID3D11ShaderResourceView*& fromDepthToSRV();
	DirectX::XMMATRIX getLightView();
	void RenderShader();
private:

	bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height);
	//object
	PointLight* light;
	Graphics* gfx;

	DirectX::XMMATRIX lightView;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	ID3D11VertexShader* vertexShadow;
	ID3D11PixelShader* pixelShadow;
	ID3D11Resource* shadowRes;
	ID3D11ShaderResourceView* shadowResV;

};