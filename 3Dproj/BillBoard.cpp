#include "BillBoard.h"
#include "Graphics.h"

BillBoard::BillBoard(Graphics*& gfx, vec3 pos, ID3D11ShaderResourceView* texSRV, ID3D11ShaderResourceView* NDef, int nrOfCol, int nrOfRow) :
	object(pos),
	points(vec3(0,0,0)),
	anim(nrOfCol, nrOfRow)
{
	this->texSRV = texSRV;
	this->NDef = NDef;
	for (int i = 0; i < 4; i++) {
		
		
	}
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreateGeometryConstBuffer(gfx, Gg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, Pg_pConstantBuffer);
	D3D11_BUFFER_DESC bDesc = {};
	bDesc.ByteWidth = sizeof(points);
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &points;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	 HRESULT hr = gfx->getDevice()->CreateBuffer(&bDesc, &data, &pointBuffer);

	if (FAILED(hr)) {
		printf("failed");
	}
}

BillBoard::~BillBoard()
{
	if (pointBuffer != nullptr) {
		pointBuffer->Release();
	}
	if (Gg_pConstantBuffer != nullptr) {
		Gg_pConstantBuffer->Release();
	}
}

void BillBoard::update(float dt)
{
	anim.update(dt);
}

void BillBoard::draw(Graphics*& gfx, bool sm)
{
	gfx->get_IC()->PSSetShaderResources(0, 1, &texSRV);
	gfx->get_IC()->PSSetShaderResources(1, 1, &texSRV);
	gfx->get_IC()->PSSetShaderResources(2, 1, &NDef);
	static UINT strid = sizeof(point);
	static UINT offset = 0;
	gfx->get_IC()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	gfx->get_IC()->GSSetConstantBuffers(0, 1, &Gg_pConstantBuffer);
	gfx->get_IC()->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	gfx->get_IC()->IASetVertexBuffers(0, 1, &pointBuffer, &strid, &offset);
	gfx->get_IC()->Draw(1, 0);
}

void BillBoard::getKdKa(float(&kd)[4], float(&ka)[4])
{
	for (int i = 0; i < 3; i++) {
		kd[i] = 0.5;
		ka[i] = 0.5;
	}
	kd[3] = 1.f;
	ka[3] = 1.f;
}

ID3D11Buffer* BillBoard::getGCB()
{
	return this->Gg_pConstantBuffer;
}

TileAnimation& BillBoard::getTAnim()
{
	return this->anim;
}

void BillBoard::UpdateShader(Graphics*& gfx, vec3 cameraPos, bool v, bool p, bool g)
{
	this->updateVertexShader(gfx);

	this->getKdKa(gfx->getPcb()->kd.element, gfx->getPcb()->ka.element);


	//changing pixel shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;
	gfx->get_IC()->Map(Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getPcb(), sizeof(Pcb));
	gfx->get_IC()->Unmap(Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//GCB
	gfx->getGcb()->cameraPos.element[0] = -cameraPos.x;
	gfx->getGcb()->cameraPos.element[1] = -cameraPos.y;
	gfx->getGcb()->cameraPos.element[2] = -cameraPos.z;

	//uv
	gfx->getGcb()->uvCords.element[0] = this->getTAnim().uv().xyz.x;
	gfx->getGcb()->uvCords.element[1] = this->getTAnim().uv().xyz.y;
	gfx->getGcb()->uvCords.element[2] = this->getTAnim().uv().xyz.z;
	gfx->getGcb()->uvCords.element[3] = this->getTAnim().uv().w;

	gfx->get_IC()->Map(this->getGCB(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getGcb(), sizeof(Gcb));
	gfx->get_IC()->Unmap(this->getGCB(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}
