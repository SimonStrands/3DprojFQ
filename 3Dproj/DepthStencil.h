#pragma once
#include <d3d11.h>
#include <string>
//TODO : ta bort?
class DepthStencil {
public:
	DepthStencil(std::string name);
    virtual ~DepthStencil();
    bool CreateDepthStencil(ID3D11Device* device, 
        UINT width, UINT height, 
        ID3D11Texture2D*& dsTexture, 
        ID3D11DepthStencilView*& dsview
    );
    bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, bool resTex);
    ID3D11ShaderResourceView* getDepthSRV();
    ID3D11DepthStencilView* getDepthStencil();
private:
    std::string name;
    ID3D11Texture2D* dsTexture;
    ID3D11ShaderResourceView* dShaderResourceView;
    ID3D11DepthStencilView* dsview;
};
