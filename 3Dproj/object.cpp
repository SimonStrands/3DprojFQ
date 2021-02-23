#include "object.h"
#include "Graphics.h"
#include <array>
#include "otherHelps.h"

object::object(std::string file, Graphics& gfx, std::string texture, vec3 pos, vec3 rot, vec3 scale)
{
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
    normalMap = true;
    if (gfx.CreateVertexBuffer(*this, file)) {
        if (texture == "") {
            texture = "stripestest.png";
        }
        if (texture != "none") {
            FileInfo fileInfo;
            fileInfo = seeIfFileExist(texture);
            if (fileInfo.exist) {
                fileName[0] = "Textures/" + texture + fileInfo.ending;
                normalMap = false;
            }
            else  {
                fileInfo = seeIfFileExist(texture + "_BaseColor");
                if (fileInfo.exist) {
                    fileName[0] = "Textures/" + texture + "_BaseColor" + fileInfo.ending;
                    fileName[1] = "Textures/" + texture + "_Normal" + fileInfo.ending;
                }
                else {
                    fileName[0] = "Texture/Default/white.png";
                    fileName[1] = "Texture/Default/white.png";
                    normalMap = false;
                }
            }
            
        }
        
    }
    else {
        printf("file doesnt exist");
    }
    gfx.MakeTexture(*this, fileName[0], 0);
    if (!gfx.MakeTexture(*this, fileName[1], 1)) {
        normalMap = false;
    }
}

object::~object()
{
    if (vertexBuffer != nullptr) {
        vertexBuffer->Release();
    }
    if (Vg_pConstantBuffer != nullptr) {
        Vg_pConstantBuffer->Release();
    }
    if (Pg_pConstantBuffer != nullptr) {
        Pg_pConstantBuffer->Release();
    }
    texSRV[0]->Release();
    if (normalMap) {
        texSRV[1]->Release();
    }
    delete[] texSRV;
}

const vec3 object::getPos()
{
    return this->pos;
}

const vec3 object::getRot()
{
    return this->rot;
}

const vec3 object::getScale()
{
    return this->scale;
}

void object::changePos(vec3 pos)
{
    this->pos = pos;
}

void object::changeRot(vec3 rot)
{
    this->rot = rot;
}

void object::changeScale(vec3 scale)
{
    this->scale = scale;
}

int& object::getNrOfVertex()
{
    return this->nrOfVertexes;
}

float& object::getxRot()
{
    return this->rot.y;
}

float& object::getxPos() {
    return this->pos.x;
}
float& object::getzPos() {
    return this->pos.z;
}

ID3D11Buffer*& object::getVertexBuffer()
{ 
    return this->vertexBuffer;
} 

ID3D11Buffer*& object::getVertexConstBuffer()
{
    return this->Vg_pConstantBuffer;
}

ID3D11Buffer*& object::getPixelConstBuffer()
{
    return this->Pg_pConstantBuffer;
}

bool object::normalMapping()
{
    return normalMap;
}


