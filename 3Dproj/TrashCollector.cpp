#include "TrashCollector.h"

TC& TC::GetInst()
{
    return s_Instance;
}

void TC::empty()
{
    //for (int i = 0; i < Matrial.size(); i++) {
    //}
}

//void TC::add(Material& matrial)
//{
//    Matrial.push_back(matrial);
//}

void TC::add(ID3D11ShaderResourceView* SRV)
{
    shaderRSV.push_back(SRV);
}

TC TC::s_Instance;