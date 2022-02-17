#include "DepthStencil.h"
#include <iostream>

DepthStencil::DepthStencil(std::string name)
{
    dsTexture = nullptr;
    dShaderResourceView = nullptr;
    dsview = nullptr;
    this->name = name;
}

DepthStencil::~DepthStencil()
{
    //if (dsTexture != nullptr) {
    //    dsTexture->Release();
    //}
    if (dShaderResourceView != nullptr) {
        dShaderResourceView->Release();
    }
    if (dsview != nullptr) {
        dsview->Release();
    }
}

bool DepthStencil::CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsview)
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

bool DepthStencil::CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, bool resTex)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    if (resTex) {
        textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    }
    else {
        textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    }
    
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (resTex ? D3D11_BIND_SHADER_RESOURCE : 0);;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
    {
        printf("failed create 2d texture");
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
    if (resTex) {
        descView.Format = DXGI_FORMAT_D32_FLOAT;
    }
    else {
        descView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    }
    
    descView.Flags = 0;
    descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descView.Texture2D.MipSlice = 0;
    HRESULT hr = device->CreateDepthStencilView(dsTexture, &descView, &dsview);
    if (hr != S_OK) {
        return !FAILED(hr);
    }
    hr = device->CreateShaderResourceView(dsTexture, nullptr, &dShaderResourceView);

    return !FAILED(hr);
}

ID3D11ShaderResourceView* DepthStencil::getDepthSRV()
{
    return dShaderResourceView;
}

ID3D11DepthStencilView* DepthStencil::getDepthStencil()
{
    return dsview;
}
