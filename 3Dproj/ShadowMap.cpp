#include "ShadowMap.h"
#include <d3d11.h>


ShadowMap::ShadowMap(Light* light, Graphics* gfx)
{
	this->light = light;
	std::string bytecode;
	loadVShader("VertexShadow.cso", gfx->getDevice(), vertexShadow, bytecode);
	loadPShader("PixelShadow.cso", gfx->getDevice(), pixelShadow);
	inputdesc(gfx, bytecode);
	sampler(gfx);
	//render target = null
	//pixel shader = null
}

bool ShadowMap::render(Graphics*& gfx, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture)
{
	bool r = setShaderParameter(gfx, worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, texture, depthMapTexture);
	
	return false;
}

bool ShadowMap::inputdesc(Graphics*& gfx, std::string& VbyteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3];
	inputDesc[0].SemanticName = "POSITION";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].InputSlot = 0;
	inputDesc[0].AlignedByteOffset = 0;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0;

	inputDesc[1].SemanticName = "TEXCOORD";
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc[1].InputSlot = 0;
	inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0;

	inputDesc[2].SemanticName = "NORMAL";
	inputDesc[2].SemanticIndex = 0;
	inputDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[2].InputSlot = 0;
	inputDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[2].InstanceDataStepRate = 0;

	int numElements = sizeof(inputDesc) / sizeof(inputDesc[0]);
	HRESULT hr = gfx->getDevice()->CreateInputLayout(inputDesc, numElements, VbyteCode.c_str(), VbyteCode.length(),
		&inputLayout);
	if (FAILED(hr)) {
		return false;
	}
	else {
		return true;
	}
}
bool ShadowMap::sampler(Graphics*& gfx)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gfx->getDevice()->CreateSamplerState(&samplerDesc, &sampleStateWrap);
	if (FAILED(hr))
	{
		return false;
	}
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	hr = gfx->getDevice()->CreateSamplerState(&samplerDesc, &sampleStateClamp);
	if (FAILED(hr))
	{
		return false;
	}
	else {
		return true;
	}
}
bool ShadowMap::buffers(Graphics*& gfx)
{
	D3D11_BUFFER_DESC mBufferDesc;
	mBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufferDesc.ByteWidth = sizeof(Vcb);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufferDesc.MiscFlags = 0;
	mBufferDesc.StructureByteStride = 0;
	HRESULT hr = gfx->getDevice()->CreateBuffer(&mBufferDesc, NULL, &matrixBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	else {
		return true;
	}
}

void ShadowMap::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(layout);
	deviceContext->VSSetShader(vertexShadow, NULL, 0);
	deviceContext->PSSetShader(pixelShadow, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &sampleStateClamp);
	deviceContext->PSSetSamplers(1, 1, &sampleStateWrap);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool ShadowMap::setShaderParameter(Graphics*& gfx, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
	DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture)
{
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	lightViewMatrix = DirectX::XMMatrixTranspose(lightViewMatrix);
	lightProjectionMatrix = DirectX::XMMatrixTranspose(lightProjectionMatrix);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vcb VdataPtr;
	SPCB PdataPtr;
	HRESULT hr = gfx->get_IC()->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		return false;
	}
	VdataPtr.view.element = viewMatrix;
	VdataPtr.transform.element = worldMatrix;
	VdataPtr.projection.element = projectionMatrix;
	VdataPtr.lightView.element = lightViewMatrix;
	VdataPtr.lightProjection.element = lightProjectionMatrix;
	memcpy(mappedResource.pData, &VdataPtr, sizeof(Vcb));
	gfx->get_IC()->Unmap(matrixBuffer, 0);
	int bufferNr = 0;
	gfx->get_IC()->VSSetConstantBuffers(bufferNr, 1, &matrixBuffer);
	gfx->get_IC()->PSSetShaderResources(0, 1, &texture);
	gfx->get_IC()->PSSetShaderResources(1, 1, &depthMapTexture);
	hr = gfx->get_IC()->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return false;
	}
	return true;

}
