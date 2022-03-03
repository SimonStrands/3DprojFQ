#include "BillBoardManager.h"
#include "Random.h"
//set position later

BillBoardManager::BillBoardManager(Graphics*& gfx, ID3D11ShaderResourceView* SRV, int maxSize, vec3 orgin, vec3 sizeofArea):
	anim()
{
	srand(time(NULL));
	//load a Computeshader
	loadCShader("BillBoardUpdate.cso", gfx->getDevice(), cUpdate);
	CompConstBuff.time.element = 0;
	for (int i = 0; i < maxSize; i++) {
		billboards.push_back(point(vec3(
			RandomNumber(orgin.x - sizeofArea.x, orgin.x + sizeofArea.x),
			RandomNumber(orgin.y - sizeofArea.y, orgin.y + sizeofArea.y),
			RandomNumber(orgin.z - sizeofArea.z, orgin.z + sizeofArea.z))));
	}
	numberOfParticles = maxSize;
	this->SRV = SRV;

	CreateVertexConstBuffer(gfx, this->Vg_pConstantBuffer);
	CreateGeometryConstBuffer(gfx, this->Gg_pConstantBuffer);
	CreatePixelConstBuffer(gfx, this->Pg_pConstantBuffer);
	CreateConstBuffer(gfx, this->Cg_pConstantBuffer, sizeof(CTCB), &CompConstBuff);

	//create UAV
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = sizeof(point) * maxSize;
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;//how does this bind with vertex Buffer?
	buffDesc.CPUAccessFlags = 0;
	buffDesc.StructureByteStride = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = billboards.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(gfx->getDevice()->CreateBuffer(&buffDesc, &data, &buff))) {
		printf("doesn't work create Buffer");
		return;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
	UavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	UavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UavDesc.Buffer.FirstElement = 0;
	UavDesc.Buffer.NumElements = maxSize * 3;
	UavDesc.Buffer.Flags = 0;
	if (FAILED(gfx->getDevice()->CreateUnorderedAccessView(buff, &UavDesc, &billUAV))) {
		printf("doesn't work create Buffer");
		return;
	}
}

void BillBoardManager::setAnimation(int noaw, int noah, float tb)
{
	this->anim.setAnimation(noaw, noah, tb);
}

BillBoardManager::~BillBoardManager()
{
	this->cUpdate->Release();
}


void BillBoardManager::update(float dt, Graphics*& gfx)
{
	this->CompConstBuff.time.element = dt;
	this->CompConstBuff.rand.element = RandomNumber(-1, 1);
	anim.update(dt);

	//dispathc shit
	gfx->get_IC()->CSSetShader(cUpdate, nullptr, 0);

	gfx->get_IC()->CSSetConstantBuffers(0, 1, &Cg_pConstantBuffer);

	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &billUAV, nullptr);

	gfx->get_IC()->Dispatch(this->billboards.size(), 1, 1);//calc how many groups we need

	//nulla unorderedaccesview
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	gfx->get_IC()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}

void BillBoardManager::updateShader(Graphics*& gfx, vec3 camPos)
{
	//doesn't make a difference
	//DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));
	vec3 scale(1, 1, 1);
	DirectX::XMMATRIX scal(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//transformPotion//this does
	DirectX::XMMATRIX trans(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x, position.y, position.z, 1.0f
	);

	DirectX::XMMATRIX rts =  (scal * trans);

	gfx->getVcb()->transform.element = rts;

	//changing vertex Shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;

	gfx->get_IC()->Map(Vg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getVcb(), sizeof(Vcb));
	gfx->get_IC()->Unmap(Vg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//GCB
	gfx->getGcb()->cameraPos.element[0] = -camPos.x;
	gfx->getGcb()->cameraPos.element[1] = -camPos.y;
	gfx->getGcb()->cameraPos.element[2] = -camPos.z;

	//uv
	gfx->getGcb()->uvCords.element[0] = anim.uv().xyz.x;
	gfx->getGcb()->uvCords.element[1] = anim.uv().xyz.y;
	gfx->getGcb()->uvCords.element[2] = anim.uv().xyz.z;
	gfx->getGcb()->uvCords.element[3] = anim.uv().w;

	gfx->get_IC()->Map(Gg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getGcb(), sizeof(Gcb));
	gfx->get_IC()->Unmap(Gg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


	//update computeshader const buffer
	gfx->get_IC()->Map(Cg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &CompConstBuff, sizeof(CTCB));
	gfx->get_IC()->Unmap(Cg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void BillBoardManager::changeBehavior()
{
}

void BillBoardManager::changeNumberOfParticles(int nrOf)
{
	this->numberOfParticles;
}

void BillBoardManager::draw(Graphics*& gfx)
{
	UINT offset = 0;
	static UINT strid = sizeof(point);

	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);

	gfx->get_IC()->PSSetShaderResources(0, 1, &SRV);

	gfx->get_IC()->VSSetConstantBuffers(0, 1, &Vg_pConstantBuffer);
	gfx->get_IC()->GSSetConstantBuffers(0, 1, &Gg_pConstantBuffer);
	gfx->get_IC()->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	
	gfx->get_IC()->IASetVertexBuffers(0, 1, &this->buff, &strid, &offset);
	gfx->get_IC()->Draw(this->numberOfParticles, 0);

}