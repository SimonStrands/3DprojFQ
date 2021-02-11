#include "Graphics.h"
#include <vector>
#include "Keyboard.h"

//debug nothing to look here at
void Graphics::debugcd()
{
}
//keyboard buttons + rotation
void Graphics::keyboardDebug()
{

}

void Graphics::createBuffer()
{
	//immediateContext->VSSetConstantBuffers(0, 2, &Vg_pConstantBuffer);
}

//not longer debug things
//#thisNeedFix
//going to make a vertexbuffer for each object?
//going to create multiple Vg_pConstantBuffer?
bool Graphics::CreateVertexBuffer(object &obj, std::string fileName)
{
	std::vector<std::vector<vertex>> vertices;
	reader.readObjFile(vertices, fileName, obj.getNrOfVertex());

	D3D11_BUFFER_DESC bDesc = {};
	bDesc.ByteWidth = sizeof(vertex) * (UINT)obj.getNrOfVertex();
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = obj.getVertecies()[0].data();
	data.pSysMem = vertices[0].data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bDesc, &data, &obj.getVertexBuffer());

	if (FAILED(hr)) {
		printf("failed");
		return false;
	}
	//create vertexConstantBuffer
	D3D11_BUFFER_DESC CbDesc;
	CbDesc.ByteWidth = sizeof(Vcb);
	CbDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CbDesc.MiscFlags = 0;
	CbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &vcbd;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&CbDesc, &InitData, &obj.getVertexConstBuffer());
	//hr = device->CreateBuffer(&CbDesc, &InitData, &obj.Vg_pConstantBuffer);
	if (FAILED(hr)) {
		printf("failed");
		return false;
	}
	//create PixelConstantBuffer (lånar vertexBuffern desc)
	CbDesc.ByteWidth = sizeof(Pcb);
	InitData.pSysMem = &pcbd;
	hr = device->CreateBuffer(&CbDesc, &InitData, &Pg_pConstantBuffer);
	UINT strid = sizeof(vertex);
	UINT offset = 0;

	//immediateContext->VSSetConstantBuffers(nrOfObject, 1, &Vg_pConstantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	nrOfObject++;

	//debug thing
	CreateTexture(obj.fileName, device, tex, obj.texSRV);

	return !FAILED(hr);

}

bool Graphics::worldMatrix()
{
	//giving pixelshade lightPos
	pcbd.lightPos.element[0] = light.getPos().x;
	pcbd.lightPos.element[1] = light.getPos().y;
	pcbd.lightPos.element[2] = light.getPos().z;
	pcbd.lightPos.element[3] = 1;

	//changing constant buffer
	D3D11_MAPPED_SUBRESOURCE resource;

	immediateContext->Map(Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &pcbd, sizeof(Pcb));
	immediateContext->Unmap(Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	return true;
}

void Graphics::updateWorldMatrix(object& obj)
{
	
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(obj.getRot().x, obj.getRot().y, obj.getRot().z));

	DirectX::XMMATRIX scal(
		obj.getScale().x, 0.0f, 0.0f, 0.0f,
		0.0f, obj.getScale().y, 0.0f, 0.0f,
		0.0f, 0.0f, obj.getScale().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//transformPotion
	DirectX::XMMATRIX trans(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		obj.getPos().x, obj.getPos().y, obj.getPos().z, 1.0f
	);

	vcbd.transform.element = ((rot * trans) * scal);

	D3D11_MAPPED_SUBRESOURCE resource;

	immediateContext->Map(obj.getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &vcbd, sizeof(Vcb));
	immediateContext->Unmap(obj.getVertexConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void Graphics::Projection()
{
	//setting projection matrix
	vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
}


Graphics::Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) :
	speed(1.5f),
	light(vec3(0.5, 0, -2.f)),
	reader()
{
	fov = 45.f;
	ratio = 16.f / 9.f;
	farPlane = 40;
	nearPlane = 0.1f;
	nrOfObject = 0;

	Pg_pConstantBuffer = NULL;
	inputLayout = nullptr; pShader = nullptr; vShader = nullptr; //texSRV = nullptr;
	//setting matrixes
	Projection();
	//if delete this happens it will get an error and program will stop working(I want this to happen when I debug)
	if (!setUpWindow(hInstance, WIDTH, HEIGHT, nCmdShow, wnd)) {
		std::cerr << "failed" << std::endl;
	}
	if (!SetupD3D11(WIDTH, HEIGHT, wnd, device, immediateContext, swapChain, renderTarget, dsTexture, dsView, viewPort, pRS))
	{
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	if (!SetupPipeline(device, vShader, pShader, inputLayout, tex, texSRV, sampler))
	{
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	
	//set settings up
	//immediateContext->PSSetShaderResources(0, 1, &texSRV);
	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewPort);
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
	immediateContext->RSSetState(pRS);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Graphics::~Graphics()
{
	if (inputLayout != nullptr) {
		inputLayout->Release();
	}
	if (vShader != nullptr) {
		vShader->Release();
	}
	if (pShader != nullptr) {
		pShader->Release();
	}
	if (dsView != nullptr) {
		dsView->Release();
	}
	if (dsTexture != nullptr) {
		dsTexture->Release();
	}
	if (renderTarget != nullptr) {
		renderTarget->Release();
	}
	if (swapChain != nullptr) {
		swapChain->Release();
	}
	if (immediateContext != nullptr) {
		immediateContext->Release();
	}
	if (device != nullptr) {
		device->Release();
	}
	if (Pg_pConstantBuffer != nullptr) {
		Pg_pConstantBuffer->Release();
	}
	if (pRS != nullptr) {
		pRS->Release();
	}
	if (tex != nullptr) {
		tex->Release();
	}
	if (texSRV != nullptr) {
		texSRV->Release();
	}
	if (sampler != nullptr) {
		sampler->Release();
	}
	delete[] objects;
}

void Graphics::setObjects(object** obj, int nrOfObjects)
{
	this->objects = new object* [nrOfObjects];
	for (int i = 0; i < nrOfObjects; i++) {
		this->objects[i] = obj[i];
	}
}

float nextFpsUpdate = 0;
void Graphics::Update(float dt)
{
	worldMatrix();
	Render();
	
	nextFpsUpdate += (float)dt;
	if (nextFpsUpdate >= 0.5f) {
		nextFpsUpdate = 0;
		float fps = 1.f / (float)dt;
		SetWindowTextA(wnd, std::to_string(fps).c_str());
	}

	keyboardDebug();
}

Vcb *Graphics::getVcb()
{
	return &vcbd;
}
Pcb* Graphics::getPcb()
{
	return &pcbd;
}
void Graphics::Render()
{
	//clear background
	float clearColor[4] = { 0.1f,0.1f,0.1f,0 };
	immediateContext->ClearRenderTargetView(renderTarget, clearColor);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	
	UINT strid = sizeof(vertex);
	UINT offset = 0;

	for (int i = 0; i < nrOfObject; i++) {
		immediateContext->PSSetShaderResources(0, 1, &objects[i]->texSRV);
		immediateContext->VSSetConstantBuffers(0, 1, &objects[i]->getVertexConstBuffer());
		immediateContext->IASetVertexBuffers(0, 1, &objects[i]->getVertexBuffer(), &strid, &offset);
		immediateContext->Draw((int)objects[i]->getNrOfVertex(), 0);
	}

	//show the "picture"
	swapChain->Present(0, 0);
}
