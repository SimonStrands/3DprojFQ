#include "TrashCollector.h"

TC& TC::GetInst()
{
    return s_Instance;
}

void TC::empty()
{
    for (int i = 0; i < shaderRSV.size(); i++) {
        shaderRSV[i]->Release();
    }
    for (int i = 0; i < Matrial.size(); i++) {
        //Matrial[i]->begone();
        delete Matrial[i];
    }
}

void TC::add(Material* matrial)
{
    Matrial.push_back(matrial);
}

void TC::add(ID3D11ShaderResourceView* SRV)
{
    shaderRSV.push_back(SRV);
}

TC TC::s_Instance;