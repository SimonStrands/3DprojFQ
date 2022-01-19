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
    scale(1,1,1),
    rPoint(0,0,0)
{
    this->pos = pos;
    Pg_pConstantBuffer = nullptr;
    Vg_pConstantBuffer = nullptr;
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

const vec3 object::getPoint()
{
    return this->rPoint;
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

void object::changePoint(vec3 point)
{
    this->rPoint = point;
}

void object::addPoint(vec3 point)
{
    this->rPoint = this->rPoint + point;
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
float& object::getyPos()
{
    return this->pos.y;
}
float& object::getzPos() {
    return this->pos.z;
}

void object::updateVertexShader(Graphics*& gfx)
{
    DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

    DirectX::XMMATRIX scal(
        this->getScale().x, 0.0f, 0.0f, 0.0f,
        0.0f, this->getScale().y, 0.0f, 0.0f,
        0.0f, 0.0f, this->getScale().z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    //transformPotion
    DirectX::XMMATRIX trans(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->getPos().x, this->getPos().y, this->getPos().z, 1.0f
    );
    DirectX::XMMATRIX point(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->getPoint().x, this->getPoint().y, this->getPoint().z, 1.0f
    );
    DirectX::XMMATRIX rts = point * (scal * (rot * trans));

    //vcbd.transform.element = rts;
    gfx->getVcb()->transform.element = rts;

    //changing vertex Shader cBuffer
    D3D11_MAPPED_SUBRESOURCE resource;

    gfx->get_IC()->Map(this->getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, gfx->getVcb(), sizeof(Vcb));
    gfx->get_IC()->Unmap(this->getVertexConstBuffer(), 0);
    ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void object::updatePixelShader(Graphics*& gfx)
{
    gfx->getPcb()->lightPos.element[0] = gfx->getLight()->getPos().x;
    gfx->getPcb()->lightPos.element[1] = gfx->getLight()->getPos().y;
    gfx->getPcb()->lightPos.element[2] = gfx->getLight()->getPos().z;
    gfx->getPcb()->lightPos.element[3] = 1;

    gfx->getPcb()->cameraPos.element[3] = this->normalMapping();//using camerapos.w a normal map on and off
    if (getkey('N')) {
        gfx->getPcb()->cameraPos.element[3] = 1;
        gfx->getPcb()->lightPos.element[3] = 3;
    }
    if (getkey('M')) {
        gfx->getPcb()->cameraPos.element[3] = 0;
        gfx->getPcb()->lightPos.element[3] = 3;
    }

    this->getKdKa(gfx->getPcb()->kd.element, gfx->getPcb()->ka.element);

    //changing pixel shader cBuffer
    D3D11_MAPPED_SUBRESOURCE resource;
    gfx->get_IC()->Map(this->getPixelConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, gfx->getPcb(), sizeof(Pcb));
    gfx->get_IC()->Unmap(this->getPixelConstBuffer(), 0);
    ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

