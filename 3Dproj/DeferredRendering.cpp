#include "DeferredRendering.h"

DeferredRendering::DeferredRendering(Graphics*& gfx)
{
	this->gfx = gfx;
	if (!InitDeferred()) {
		std::cout << "stop" << std::endl;
	}
	//set compute shader and a extra test for pixel shader
	std::string a;
	//load cs
	loadCShader("DeferredLightningCS.cso", gfx->getDevice(), DeferredComputeS);
	loadPShader("TestFragmentshader.cso", gfx->getDevice(), DeferredPixelS);
}

DeferredRendering::~DeferredRendering()
{
	for (int i = 0; i < 3; i++) {
		DeferredResV[i]->Release();
		DeferredTex[i]->Release();
		DeferredRTV[i]->Release();
	}
}

//code helped from Hilze Vonck
//https://www.youtube.com/watch?v=2ThW4Gz6oYM
bool DeferredRendering::InitDeferred()
{	


	//change so depth is 1 float???
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = (UINT)gfx->getWH().x;
	textureDesc.Height = gfx->getWH().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (int i = 0; i < 3; i++) {
		if (FAILED(gfx->getDevice()->CreateTexture2D(&textureDesc, NULL, &DeferredTex[i]))) {
			printf("doesn't work tex2d");
			return false;
		}
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (int i = 0; i < 3; i++) {
		if (FAILED(gfx->getDevice()->CreateRenderTargetView(DeferredTex[i], &renderTargetViewDesc, &DeferredRTV[i]))) {
			printf("doesn't work RTV");
			return false;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	for (int i = 0; i < 3; i++) {
		if (FAILED(gfx->getDevice()->CreateShaderResourceView(DeferredTex[i], &shaderResourceViewDesc, &DeferredResV[i])))
		{
			printf("failed create RSV");
			return false;
		}
	}
	ID3D11Texture2D* depthStencilTex = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc{};
	depthStencilBufferDesc.Width = gfx->getWH().x;
	depthStencilBufferDesc.Height = gfx->getWH().y;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(gfx->getDevice()->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilTex))) 
	{
		printf("failed create tex2d depth");
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
	DSVDesc.Format = depthStencilBufferDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	if (FAILED(gfx->getDevice()->CreateDepthStencilView(depthStencilTex, &DSVDesc, &this->dsView))) {
		printf("DEPTH STENCIL VIEW");
		return false;
	}

	depthStencilTex->Release();	
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(gfx->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		printf("failed to swapchain");
		return false;
	}

	HRESULT hr = gfx->getDevice()->CreateUnorderedAccessView(backBuffer, nullptr, &UAV);
	backBuffer->Release();
	return !FAILED(hr);
}

void DeferredRendering::BindFirstPass()
{
	//SET INPUT LAYOUT
	FLOAT color[4] = { 0.1f,0.1f,0.1f,1.f };
	//gfx->get_IC()->RSSetViewports(1, &viewPort);//?
	//set render targets
	gfx->get_IC()->OMSetRenderTargets(3, DeferredRTV, gfx->getDepthStencil());
	gfx->get_IC()->ClearRenderTargetView(DeferredRTV[0], color);
	gfx->get_IC()->ClearRenderTargetView(DeferredRTV[1], color);
	gfx->get_IC()->ClearRenderTargetView(DeferredRTV[2], color);
	//gfx->get_IC()->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH, 1.0f, 0);


}

void DeferredRendering::BindSecondPass(ID3D11ShaderResourceView*& ShadowMapping)
{
	FLOAT color[4] = { 0.1f,0.1f,0.1f,1.f };
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	ID3D11RenderTargetView* nullRTV[3] = { nullptr };
	//set till null?
	gfx->get_IC()->OMSetRenderTargets(3, nullRTV, nullptr);

	//is this for compute shading
	gfx->get_IC()->CSSetShader(DeferredComputeS, nullptr, 0);

	gfx->get_IC()->CSSetShaderResources(0, 3, DeferredResV);//add ShadowMapping
	gfx->get_IC()->CSSetShaderResources(3, 1, &ShadowMapping);
	
	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &this->UAV, nullptr);
	//k�ra computeShader
	gfx->get_IC()->Dispatch(60, 135, 1);
	ID3D11ShaderResourceView* nullSRV[4] = { nullptr };
	gfx->get_IC()->CSSetShaderResources(0, _countof(nullSRV), nullSRV);
	//nulla unorderedaccesview
	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

ID3D11PixelShader* DeferredRendering::getPS()
{
	return this->DeferredPixelS;
}