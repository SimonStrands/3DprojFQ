#include "object.h"
#include "Graphics.h"

object::object(std::string file, Graphics& gfx, vec3 pos, vec3 rot, vec3 scale)
{
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
    gfx.CreateVertexBuffer(*this, file);
}

object::~object()
{
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

std::vector<std::vector<vertex>>& object::getVertecies()
{
    return vertices;
}
