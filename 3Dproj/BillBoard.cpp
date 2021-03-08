#include "BillBoard.h"
#include "Graphics.h"

BillBoard::BillBoard(Graphics*& gfx, vec3 pos, ID3D11ShaderResourceView* texSRV, int nrOfCol, int nrOfRow) :
	object(pos)
{
	this->texSRV = texSRV;
	CreateVertexConstBuffer(gfx, Vg_pConstantBuffer);
	CreateGeometryConstBuffer(gfx, Gg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, Pg_pConstantBuffer);
	point test(pos);
	D3D11_BUFFER_DESC bDesc = {};
	bDesc.ByteWidth = sizeof(point);
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &test;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	 HRESULT hr = gfx->getDevice()->CreateBuffer(&bDesc, &data, &pointBuffer);

	if (FAILED(hr)) {
		printf("failed");
	}
}

void BillBoard::update()
{
}

void BillBoard::draw(ID3D11DeviceContext*& immediateContext)
{
	immediateContext->PSGetShaderResources(0, 1, &texSRV);
	immediateContext->VSSetConstantBuffers(0, 1, &Vg_pConstantBuffer);
	immediateContext->GSSetConstantBuffers(0, 1, &Gg_pConstantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	//immediateContext->IASetVertexBuffers(0, 1, &mesh->getVertexBuffer(), &strid, &offset);
	immediateContext->Draw(1, 0);
}

ID3D11Buffer* BillBoard::getVCB()
{
	return this->Vg_pConstantBuffer;
}

ID3D11Buffer* BillBoard::getGCB()
{
	return this->Gg_pConstantBuffer;
}

ID3D11Buffer* BillBoard::getPCB()
{
	return this->Pg_pConstantBuffer;
}
