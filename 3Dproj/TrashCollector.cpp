#include "TrashCollector.h"

TC& TC::GetInst()
{
    return s_Instance;
}

void TC::empty()
{
    for (int i = 0; i < shaderRSV.size(); i++) {
        if (shaderRSV[i] != nullptr) {
            shaderRSV[i]->Release();
        }
    }
}

void TC::add(ID3D11ShaderResourceView* SRV)
{
    shaderRSV.push_back(SRV);
}

TC TC::s_Instance;