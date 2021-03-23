#include "object.h"
#include "Graphics.h"
#include <array>
#include "otherHelps.h"

object::object()
{
    Pg_pConstantBuffer = nullptr;
    Vg_pConstantBuffer = nullptr;
}

object::object(vec3 pos):
    rot(0,0,0),
    scale(1,1,1)
{
    this->pos = pos;
}

object::~object()
{
    if (Vg_pConstantBuffer != nullptr) {
        Vg_pConstantBuffer->Release();
    };
    if (Pg_pConstantBuffer != nullptr) {
        Pg_pConstantBuffer->Release();
    };
}

vec3 object::getPos()
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

void object::addPos(vec3 pos)
{
    this->pos = this->pos + pos;
}

void object::addRot(vec3 rot)
{
    this->rot = this->rot + rot;
}

void object::addScale(vec3 scale)
{
    this->scale = this->scale + scale;
}

ID3D11Buffer*& object::getVertexConstBuffer()
{
    return this->Vg_pConstantBuffer;
}

ID3D11Buffer*& object::getPixelConstBuffer()
{
    return this->Pg_pConstantBuffer;
}

float& object::normalMapping()
{
    return this->normalMap;
}

void object::getKdKa(float kd[4], float ka[4])
{
    for (int i = 0; i < 4; i++) {
        kd[i] = this->kd[i];
        ka[i] = this->ka[i];
    }
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


