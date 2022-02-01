#pragma once
#include <d3d11.h>
#include "Graphics.h"

class DeferredRendering {
public:
	DeferredRendering(Graphics*& gfx);
	~DeferredRendering();
	void BindFirstPass();
	void BindSecondPass(ID3D11ShaderResourceView*& ShadowMapping);
	ID3D11PixelShader* getPS();
private:
	//functions
	bool InitDeferred();
	

	//objects/classes
	Graphics* gfx;

	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	ID3D11Resource* DeferredRes;
	D3D11_VIEWPORT viewPort;

	ID3D11PixelShader* DeferredPixelS;//an extra
	ID3D11ComputeShader* DeferredComputeS;

	ID3D11UnorderedAccessView* UAV;//till Compute shadern
	//behöver ändra denna till nullptr senare

	ID3D11ShaderResourceView*	DeferredResV[3];//need a forth for normal map?
	ID3D11Texture2D*			DeferredTex[3];
	ID3D11RenderTargetView*		DeferredRTV[3];
};