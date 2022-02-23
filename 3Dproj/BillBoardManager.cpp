#include "BillBoardManager.h"

BillBoardManager::BillBoardManager(Graphics*& gfx)
{
	//load a Computeshader

	//create UAV
	D3D11_BUFFER_DESC buffDesc;
	//texDesc.ByteWidth = ;//sizeof(point);
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;//how does this bind with vertex Buffer?
	buffDesc.CPUAccessFlags = 0;
	buffDesc.StructureByteStride = 0;
	buffDesc.MiscFlags = 0;

	if (FAILED(gfx->getDevice()->CreateBuffer(&buffDesc, NULL, &buff))) {
		printf("doesn't work create Buffer");
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
	UavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	UavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UavDesc.Buffer.FirstElement = 0;
	//UavDesc.Buffer.NumElements = std::size(point)*3//
	UavDesc.Buffer.Flags = 0;

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

	gfx->get_IC()->CSSetShaderResources(0, 1, nullptr);//no resources

	//gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &UAV, nullptr);//need to create a UAV
	//köra computeShader
	//gfx->get_IC()->Dispatch(dx, dy, 1);//calc how many groups we need

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
