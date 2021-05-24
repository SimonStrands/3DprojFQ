#pragma once
#include <d3d11.h>

class DepthStencil {
public:
	DepthStencil();
    bool CreateDepthStencil(ID3D11Device* device, 
        UINT width, UINT height, 
        ID3D11Texture2D*& dsTexture, 
        ID3D11DepthStencilView*& dsview
    );
    bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, bool resTex);
    ID3D11ShaderResourceView* getDepthSRV();
    ID3D11DepthStencilView* getDepthStencil();
private:
    ID3D11Texture2D* dsTexture;
    ID3D11ShaderResourceView* dShaderResourceView;
    ID3D11DepthStencilView* dsview;
};

/*
bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsview)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
    {
        printf("failed create 2d texture");
        return false;
    }

    HRESULT hr = device->CreateDepthStencilView(dsTexture, nullptr, &dsview);
    return !FAILED(hr);
}
*/