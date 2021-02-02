#include "Graphics.h"
#include <wrl.h>
#include <vector>
#include "Keyboard.h"
//git
//debug nothing to look here at
void Graphics::debugcd()
{
}
//keyboard buttons + rotation
void Graphics::keyboardDebug()
{
	if (getkey('X')) {
		yRot -= speed * 0.001f;
	}
	if (getkey('Z')) {
		yRot += speed * 0.001f;
	}
}

//not longer debug things
//#thisNeedFix
//going to make a vertexbuffer for each object?
//going to create multiple Vg_pConstantBuffer?
bool Graphics::CreateVertexBuffer()
{
	std::vector<vertex> obj;
	reader.readObjFile(obj, "obj/objtest.obj", nrOfTriangles);


	D3D11_BUFFER_DESC bDesc = {};
	bDesc.ByteWidth = sizeof(vertex) * obj.size();
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = obj.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bDesc, &data, &vertexBuffer);

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

	hr = device->CreateBuffer(&CbDesc, &InitData, &Vg_pConstantBuffer);
	if (FAILED(hr)) {
		printf("failed");
		return false;
	}
	//create PixelConstantBuffer (lånar vertexBuffern desc)
	CbDesc.ByteWidth = sizeof(Pcb);
	InitData.pSysMem = &pcbd;
	hr = device->CreateBuffer(&CbDesc, &InitData, &Pg_pConstantBuffer);

	return !FAILED(hr);

}

bool Graphics::worldMatrix()
{
	//Y rotation
	DirectX::XMMATRIX rot(
		std::cos(yRot), 0.0f, std::sin(yRot), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-std::sin(yRot), 0.0f, std::cos(yRot), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//scale:ing
	DirectX::XMMATRIX scal(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//transformPotion
	DirectX::XMMATRIX trans(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		quadpos.x, quadpos.y, quadpos.z, 1.0f
	);
	DirectX::XMMATRIX rotPos(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		quadRoationPos.x, quadRoationPos.y, quadRoationPos.z, 1.0f
	);
	//creating model Matrix
	vcbd.transform.element = rotPos * ((rot * trans) * scal);

	//giving pixelshade lightPos
	pcbd.lightPos.element[0] = light.getPos().x;
	pcbd.lightPos.element[1] = light.getPos().y;
	pcbd.lightPos.element[2] = light.getPos().z;
	pcbd.lightPos.element[3] = 1;

	//changing constant buffer
	D3D11_MAPPED_SUBRESOURCE resource;

	immediateContext->Map(Vg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &vcbd, sizeof(Vcb));
	immediateContext->Unmap(Vg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	immediateContext->Map(Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &pcbd, sizeof(Pcb));
	immediateContext->Unmap(Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	return true;
}

void Graphics::Projection()
{
	//setting projection matrix
	vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
}


Graphics::Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) :
	makeToCb(vcbd.transform.element),
	speed(1.5f),
	light(vec3(0.5, 0, -2.f)),
	quadpos(0, 0, 0),
	quadRoationPos(0.f, 0.f, -2.0f),
	reader()
{

	if (!setUpWindow(hInstance, WIDTH, HEIGHT, nCmdShow, wnd)) {
		std::cerr << "failed" << std::endl;
	}

	fov = 45.f;
	ratio = 16.f / 9.f;
	farPlane = 40;
	nearPlane = 0.1f;
	
	yRot = 0;
	scale = 1;
	nrOfTriangles = 0;

	Vg_pConstantBuffer = NULL;
	Pg_pConstantBuffer = NULL;
	inputLayout = nullptr; pShader = nullptr; vShader = nullptr; vertexBuffer = nullptr; texSRV = nullptr;
	//setting matrixes
	Projection();
	//if delete this happens it will get an error and program will stop working(I want this to happen when I debug)
	if (!SetupD3D11(WIDTH, HEIGHT, wnd, device, immediateContext, swapChain, renderTarget, dsTexture, dsView, viewPort, pRS))
	{
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, tex, texSRV, sampler))
	{
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	if (!CreateVertexBuffer()) {
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	
	UINT strid = sizeof(vertex);
	UINT offset = 0;
	//set settings up
	immediateContext->PSSetShaderResources(0, 1, &texSRV);
	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strid, &offset);
	immediateContext->RSSetViewports(1, &viewPort);
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
	immediateContext->RSSetState(pRS);
	immediateContext->VSSetConstantBuffers(0, 1, &Vg_pConstantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &Pg_pConstantBuffer);
	immediateContext->IASetInputLayout(inputLayout);
	//immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
}

Graphics::~Graphics()
{
	if (vertexBuffer != nullptr) {
		vertexBuffer->Release();
	}
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
	if (Vg_pConstantBuffer != nullptr) {
		Vg_pConstantBuffer->Release();
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
}

int Graphics::getNrOfTriangles()
{
	return this->nrOfTriangles;
}

float nextFpsUpdate = 0;
void Graphics::Update(DeltaTime dt)
{
	worldMatrix();
	Render();
	
	nextFpsUpdate += dt.dt();
	if (nextFpsUpdate >= 0.5f) {
		nextFpsUpdate = 0;
		float fps = 1.f / (float)dt.dt();
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
	

	//#thisNeedFix
	//for (int i = 0; i < nrOfTriangles/2; i++) {
	//	immediateContext->Draw(3, (i) * 4);
	//	immediateContext->Draw(3, (i) * 4 + 1);
	//}
	immediateContext->Draw(nrOfTriangles*3, 0);
	//show the "picture"
	swapChain->Present(0, 0);
}
