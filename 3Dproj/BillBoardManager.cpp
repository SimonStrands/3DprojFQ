#include "BillBoardManager.h"

BillBoardManager::BillBoardManager(Graphics*& gfx)
{
	//load a Computeshader

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
	UavDesc.Buffer.NumElements = billboards.size();
	UavDesc.Buffer.Flags = 0;
	if (FAILED(gfx->getDevice()->CreateUnorderedAccessView(buff, &UavDesc, &billUAV))) {
		printf("doesn't work create Buffer");
		return;
	}
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
	gfx->get_IC()->CSSetShader(cUpdate, nullptr, 0);

	//gfx->get_IC()->CSSetConstantBuffers(0,1,nullptr);

	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &billUAV, nullptr);//need to create a UAV
	//köra computeShader
	//change the x value to something good later
	gfx->get_IC()->Dispatch(1, 1, 1);//calc how many groups we need

	//nulla unorderedaccesview
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

void BillBoardManager::changeBehavior()
{
}

void BillBoardManager::draw(Graphics*& gfx)
{
}
