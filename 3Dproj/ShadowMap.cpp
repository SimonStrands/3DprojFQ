#include "ShadowMap.h"
#include "Graphics.h"
#include "rotation.h"


ShadowMap::ShadowMap(PointLight* light, Graphics* gfx)
{
	this->gfx = gfx;
	this->light = light;
	std::string a;
	loadVShader("VertexShadow.cso", gfx->getDevice(), vertexShadow, a);
	loadPShader("PixelShadow.cso", gfx->getDevice(), pixelShadow);
	CreateDepthStencil(gfx->getDevice(), gfx->getWH().x, gfx->getWH().y);

	fromDepthToSRV();
}

ID3D11DepthStencilView* ShadowMap::Getdepthview()
{
	return this->dsView;
}

ID3D11ShaderResourceView* ShadowMap::GetshadowResV()
{
	return this->shadowResV;
}

void ShadowMap::RenderShader()
{
	gfx->get_IC()->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	gfx->get_IC()->VSSetShader(vertexShadow, nullptr, 0);
	gfx->get_IC()->PSSetShader( pixelShadow, nullptr, 0);

	
	DirectX::XMMATRIX temp(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-light->getPos().x, -light->getPos().y, -light->getPos().z, 1.0f
	);
	XRotation(temp, light->getRotation().x);
	YRotation(temp, light->getRotation().y);
	lightView = temp;

}

ID3D11ShaderResourceView*& ShadowMap::fromDepthToSRV()
{
	dsView->GetResource(&shadowRes);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	HRESULT hr = gfx->getDevice()->CreateShaderResourceView(
		shadowRes, &srvDesc, &shadowResV
	);
	if (hr != S_OK) {
		printf("can create shadowResourceView");
	}
	return shadowResV;
}

DirectX::XMMATRIX ShadowMap::getLightView()
{
	return this->lightView;
}

bool ShadowMap::CreateDepthStencil(ID3D11Device* device, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	descView.Format = DXGI_FORMAT_D32_FLOAT;
	descView.Flags = 0;
	descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descView.Texture2DArray.MipSlice = 0;
	descView.Texture2DArray.ArraySize = 1;
	descView.Texture2DArray.FirstArraySlice = (UINT)0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		printf("failed create 2d texture");
		return false;
	}
	HRESULT hr = device->CreateDepthStencilView(dsTexture, &descView, &dsView);
	return !FAILED(hr);
}

