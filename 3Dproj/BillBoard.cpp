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
		ka[i] = 1;
		kd[i] = 1;
	}
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	CreateGeometryConstBuffer(gfx, Gg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, this->getPixelConstBuffer());
	vertex a(std::array<float, 3>{0, 0, 5}, std::array<float, 2>{0, 0}, std::array<float, 4>{1, 1, 1, 1});
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
	this->normalMap = 0.f;
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

void BillBoard::draw(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->PSSetShaderResources(0, 1, &texSRV);
	immediateContext->PSSetShaderResources(1, 1, &texSRV);
	immediateContext->PSSetShaderResources(2, 1, &NDef);
	static UINT strid = sizeof(point);
	static UINT offset = 0;
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->GSSetConstantBuffers(0, 1, &Gg_pConstantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &this->getPixelConstBuffer());
	immediateContext->IASetVertexBuffers(0, 1, &pointBuffer, &strid, &offset);
	immediateContext->Draw(1, 0);
}

ID3D11Buffer* BillBoard::getGCB()
{
	return this->Gg_pConstantBuffer;
}

TileAnimation& BillBoard::getTAnim()
{
	return this->anim;
}
