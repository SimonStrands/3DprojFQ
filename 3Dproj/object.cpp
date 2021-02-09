#include "object.h"
#include "Graphics.h"

object::object(std::string file, Graphics& gfx, std::string texture, vec3 pos, vec3 rot, vec3 scale)
{
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
    if(texture == ""){
        texture = "stripestest.png";
    }
    fileName = "Textures/" + texture;
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

int& object::getNrOfVertex()
{
    return this->nrOfVertexes;
}

ID3D11Buffer*& object::getVertexBuffer()
{
    return this->vertexBuffer;
} 

ID3D11Buffer*& object::getVertexConstBuffer()
{
    return this->Vg_pConstantBuffer;
}


