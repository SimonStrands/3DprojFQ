#include "object.h"
#include "Graphics.h"
#include <array>

object::object(std::string file, Graphics& gfx, std::string texture, vec3 pos, vec3 rot, vec3 scale)
{
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
    if(texture == ""){
        texture = "stripestest.png";
    }
    if (texture != "none") {
        fileName[0] = "Textures/" + texture + "_BaseColor.jpg";
        fileName[1] = "Textures/" + texture + "_Normal.jpg";
    }
    else {
        fileName[0] = "Texture/Default/white.png";
        fileName[1] = "Texture/Default/white.png";
    }

    gfx.CreateVertexBuffer(*this, file);
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
    for (int i = 0; i < 2; i++) {
        texSRV[i]->Release();
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


