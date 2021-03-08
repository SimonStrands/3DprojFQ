#include "object.h"
#include "Graphics.h"
#include <array>
#include "otherHelps.h"

object::object()
{
}

object::object(vec3 pos)
{
    this->pos = pos;
}

object::~object()
{
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

/*int& object::getNrOfVertex()
{
    return this->nrOfVertexes;
}*/

/*Mesh& object::getMesh()
{
    return mesh;
}*/

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

/*ID3D11Buffer*& object::getVertexBuffer()
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

bool &object::normalMapping()
{
    return normalMap;
}*/


