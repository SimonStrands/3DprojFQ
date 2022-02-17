#pragma once
#include <d3d11.h>
#include "Graphics.h"

class DeferredRendering {
public:
	DeferredRendering(Graphics*& gfx);
	~DeferredRendering();
	void BindFirstPass();
	void BindSecondPass(ID3D11ShaderResourceView*& ShadowMapping);
	void BindSecondPassFunc(
		ID3D11ShaderResourceView*& ShadowMapping, 
		ID3D11UnorderedAccessView* UAV,
		int dx = 60, int dy = 135
	);
	ID3D11PixelShader* getPS();
private:
	//functions
	bool InitDeferred();
	
	int nrOfRTV;
	//objects/classes
	Graphics* gfx;

	ID3D11DepthStencilView* dsView;
	//ID3D11Resource* DeferredRes;
	D3D11_VIEWPORT viewPort;

	ID3D11PixelShader* DeferredPixelS;//an extra
	ID3D11ComputeShader* DeferredComputeS;

	ID3D11UnorderedAccessView* UAV;//till Compute shadern
	//behöver ändra denna till nullptr senare

	ID3D11ShaderResourceView*	DeferredResV[5];//0 = pos 1 = normal 2 = dif 3 = kakdksNS 4 = map_ka 5 = map_ks
	ID3D11Texture2D*			DeferredTex[5];
	ID3D11RenderTargetView*		DeferredRTV[5];
};