#include "DynamicCubeEnviroment.h"

DynamicCube::DynamicCube(ModelObj* model, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale):
	GameObject(model, gfx, pos, rot, scale)
{
	//set PS VS GS DS
	for (int i = 0; i < this->model->getMehses().size(); i++) {
		this->model->getMehses()[i].SetShaders((ID3D11HullShader*)nullptr, (ID3D11DomainShader*) nullptr);
		this->model->getMehses()[i].SetShaders(gfx->getVS()[0], gfx->getPS()[3]);
	}
	loadCShader("DeffrendCSwithUAVArray.cso", gfx->getDevice(), CSShader);
	initCubeMapping(gfx);
	CreateConstBuffer(gfx, CBbuffer, sizeof(DCCB), nullptr);
}

void DynamicCube::draw(ID3D11DeviceContext*& immediateContext)
{	
	immediateContext->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	static UINT strid = sizeof(vertex);
	immediateContext->PSSetShaderResources(0, 1, &this->CubeResV);

	for (int i = 0; i < this->model->getMehses().size(); i++) {
		this->model->getMehses()[i].draw2(immediateContext);
	}
}

ID3D11UnorderedAccessView** DynamicCube::getUAVs()
{
	return this->UAVs;
}

ID3D11ComputeShader *DynamicCube::getCSShader()
{
	return this->CSShader;
}

void DynamicCube::update(vec3 camPos, Graphics*& gfx)
{
	this->ConstBufferElements.cameraPos.element[0] = camPos.x;
	this->ConstBufferElements.cameraPos.element[1] = camPos.y;
	this->ConstBufferElements.cameraPos.element[2] = camPos.z;
	this->ConstBufferElements.cameraPos.element[3] = 0;

	D3D11_MAPPED_SUBRESOURCE resource;
	gfx->get_IC()->Map(this->CBbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &ConstBufferElements, sizeof(DCCB));
	gfx->get_IC()->Unmap(this->CBbuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gfx->get_IC()->PSSetConstantBuffers(0, 1, &CBbuffer);
}

bool DynamicCube::initCubeMapping(Graphics*& gfx)
{
	const int nrOfRTV = 6;
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = 640;
	textureDesc.Height = 640;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = nrOfRTV;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	if (FAILED(gfx->getDevice()->CreateTexture2D(&textureDesc, NULL, &CubeTex))) {
		printf("doesn't work tex2d");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC RSVdesc{};
	RSVdesc.Format = textureDesc.Format;
	RSVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RSVdesc.Texture2DArray.FirstArraySlice = 0;
	RSVdesc.Texture2DArray.MostDetailedMip = 0;
	RSVdesc.Texture2DArray.MipLevels = 1;
	RSVdesc.Texture2DArray.ArraySize = nrOfRTV;

	HRESULT hr = gfx->getDevice()->CreateShaderResourceView(CubeTex, &RSVdesc, &CubeResV);
	if (hr != S_OK)
	{
		printf("failed create RSV");
		return false;
	}

	UAVs = new ID3D11UnorderedAccessView * [nrOfRTV];
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
	UAVdesc.Format = textureDesc.Format;
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	UAVdesc.Texture2DArray.MipSlice = 0;
	UAVdesc.Texture2DArray.ArraySize = 1;
	
	for (int i = 0; i < nrOfRTV; i++) {
		UAVdesc.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);
		HRESULT hr = gfx->getDevice()->CreateUnorderedAccessView(CubeTex, &UAVdesc, &UAVs[i]);
		if (hr != S_OK) {
			printf("doesn't work");
			return false;
		}
	}

	return true;
}
