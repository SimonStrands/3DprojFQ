#include "BillBoardManager.h"

BillBoardManager::BillBoardManager(Graphics*& gfx, int maxSize)
{
	//load a Computeshader
	loadCShader("BillBoardUpdate.cso", gfx->getDevice(), cUpdate);

	//create UAV
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = sizeof(point);
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;//how does this bind with vertex Buffer?
	buffDesc.CPUAccessFlags = 0;
	buffDesc.StructureByteStride = 0;
	buffDesc.MiscFlags = 0;

	if (FAILED(gfx->getDevice()->CreateBuffer(&buffDesc, NULL, &buff))) {
		printf("doesn't work create Buffer");
		return;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
	UavDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	UavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UavDesc.Buffer.FirstElement = 0;
	UavDesc.Buffer.NumElements = maxSize;
	UavDesc.Buffer.Flags = 0;
	if (FAILED(gfx->getDevice()->CreateUnorderedAccessView(buff, &UavDesc, &billUAV))) {
		printf("doesn't work create Buffer");
		return;
	}

	//D3D11_BUFFER_DESC bDesc = {};
	//bDesc.ByteWidth = sizeof(points);
	//bDesc.Usage = D3D11_USAGE_DYNAMIC;
	//bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//bDesc.MiscFlags = 0;
	//bDesc.StructureByteStride = 0;
	//
	//D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = &points;
	//data.SysMemPitch = 0;
	//data.SysMemSlicePitch = 0; 
	//
	//if (FAILED(gfx->getDevice()->CreateBuffer(&bDesc, &data, &pointBuffer))) {
	//	printf("failed");
	//}
}

BillBoardManager::~BillBoardManager()
{
	for (int i = 0; i < billboards.size(); i++) {
		delete billboards[i];
	}
	this->cUpdate->Release();
}

void BillBoardManager::addBillBoard(BillBoard *bill)
{
	billboards.push_back(bill);
}

void BillBoardManager::update(float dt, Graphics*& gfx)
{
	for (int i = 0; i < billboards.size(); i++) {
		billboards[i]->update(dt);
	}


	//dispathc shit
	gfx->get_IC()->CSSetShader(cUpdate, nullptr, 0);

	//gfx->get_IC()->CSSetConstantBuffers(0,1,nullptr);

	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &billUAV, nullptr);

	gfx->get_IC()->Dispatch(this->billboards.size(), 1, 1);//calc how many groups we need

	//nulla unorderedaccesview
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

void BillBoardManager::updateShader(Graphics*& gfx, vec3 camPos)
{
	for (int i = 0; i < billboards.size(); i++) {
		billboards[i]->UpdateShader(gfx, camPos,false, true, true);
	}
}

void BillBoardManager::changeBehavior()
{
}

void BillBoardManager::draw(Graphics*& gfx)
{
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < billboards.size(); i++) {
		billboards[i]->draw(gfx);
	}
}
