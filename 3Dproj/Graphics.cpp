﻿#include "Graphics.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Camera.h"
//debug nothing to look here at
void Graphics::debugcd()
{
}
//keyboard buttons + rotation
void Graphics::keyboardDebug()
{
	if (getkey('N') && !pressed) {
		pressed = true;
		if (normalMapping == true) {
			normalMapping = false;
		}
		else {
			normalMapping = true;
		}
		//pcbd.nMapping.element = normalMapping;
	}
	else {
		pressed = false;
	}
}

void Graphics::updateShaders(GameObject& obj)
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
	DirectX::XMMATRIX rts = ((rot * trans) * scal);

	vcbd.transform.element = rts;

	//changing vertex Shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;

	immediateContext->Map(obj.getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &vcbd, sizeof(Vcb));
	immediateContext->Unmap(obj.getVertexConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


	//giving pixelshade lightPos
	pcbd.lightPos.element[0] = light.getPos().x;
	pcbd.lightPos.element[1] = light.getPos().y;
	pcbd.lightPos.element[2] = light.getPos().z;
	pcbd.lightPos.element[3] = 1;

	//changing pixel shader cBuffer
	std::cout << "stop" << std::endl;

	immediateContext->Map(obj.getPixelConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	std::cout << "hello" << std::endl;
	memcpy(resource.pData, &pcbd, sizeof(Pcb));
	immediateContext->Unmap(obj.getPixelConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void Graphics::updateVertexShader(object& obj)
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
	DirectX::XMMATRIX point(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	DirectX::XMMATRIX rts = point * (scal*(rot * trans));

	vcbd.transform.element = rts;

	//changing vertex Shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;

	immediateContext->Map(obj.getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &vcbd, sizeof(Vcb));
	immediateContext->Unmap(obj.getVertexConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void Graphics::updateGeometryShader(BillBoard& obj, Camera cam)
{
	gcbd.cameraPos.element[0] = -cam.getPos().x;
	gcbd.cameraPos.element[1] = -cam.getPos().y;
	gcbd.cameraPos.element[2] = -cam.getPos().z;

	//uv
	gcbd.uvCords.element[0] = obj.getTAnim().uv().xyz.x;
	gcbd.uvCords.element[1] = obj.getTAnim().uv().xyz.y;
	gcbd.uvCords.element[2] = obj.getTAnim().uv().xyz.z;
	gcbd.uvCords.element[3] = obj.getTAnim().uv().w;

	D3D11_MAPPED_SUBRESOURCE resource;
	immediateContext->Map(obj.getGCB(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &gcbd, sizeof(Gcb));
	immediateContext->Unmap(obj.getGCB(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void Graphics::updatePixelShader(object& obj)
{
	pcbd.lightPos.element[0] = light.getPos().x;
	pcbd.lightPos.element[1] = light.getPos().y;
	pcbd.lightPos.element[2] = light.getPos().z;
	pcbd.lightPos.element[3] = 1;

	obj.getKdKa(pcbd.kd.element, pcbd.ka.element);

	//changing pixel shader cBuffer
	D3D11_MAPPED_SUBRESOURCE resource;
	immediateContext->Map(obj.getPixelConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &pcbd, sizeof(Pcb));
	immediateContext->Unmap(obj.getPixelConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

}

void Graphics::Projection()
{
	//setting projection matrix
	vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
}

Graphics::Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) :
	speed(1.5f),
	light(vec3(0.f, 0.f, 40.f))
{
	fov = 45.f;
	ratio = 16.f / 9.f;
	farPlane = 200.f;
	nearPlane = 0.1f;
	nrOfObject = 0;
	Pg_pConstantBuffer = NULL;
	normalMapping = true;
	inputLayout = new ID3D11InputLayout * [2];
	vShader = new ID3D11VertexShader * [2];
	gShader = new ID3D11GeometryShader * [1];
	pShader = new ID3D11PixelShader * [1];
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
	if (!SetupPipeline(device, vShader, pShader, gShader, inputLayout, tex, sampler))
	{
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	D3D11_BLEND_DESC bd = {};
	bs = nullptr;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&bd, &bs);
	UINT a = 0xFFFFFFFFu;
	immediateContext->OMSetBlendState(bs, nullptr, 0xFFFFFFFFu);
	
	//set settings up
	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->VSSetShader(vShader[0], nullptr, 0);
	immediateContext->GSSetShader(gShader[0], nullptr, 0);
	immediateContext->PSSetShader(pShader[0], nullptr, 0);
	immediateContext->RSSetViewports(1, &viewPort);
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
	immediateContext->RSSetState(pRS);
	immediateContext->IASetInputLayout(inputLayout[0]);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ImGui_ImplDX11_Init(device, immediateContext);
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	shutDownWindow();
	
	if (pShader != nullptr) {
		pShader[0]->Release();
	}
	if (gShader != nullptr) {
		gShader[0]->Release();
	}
	for (int i = 0; i < 2; i++) {
		if (vShader[i] != nullptr) {
			vShader[i]->Release();
		}
		if (inputLayout[i] != nullptr) {
			inputLayout[i]->Release();
		}
		
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
	if (sampler != nullptr) {
		sampler->Release();
	}
	delete[] objects;
	if (bs != nullptr) {
		bs->Release();
	}
}

float nextFpsUpdate = 0;
void Graphics::Update(float dt)
{
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
Pcb *Graphics::getPcb()
{
	return &pcbd;
}
ID3D11Device* Graphics::getDevice()
{
	return this->device;
}
ID3D11DeviceContext*& Graphics::get_IC()
{
	return this->immediateContext;
}
ID3D11Texture2D*& Graphics::getTexture()
{
	return tex;
}
ID3D11VertexShader** Graphics::getVS()
{
	return this->vShader;
}
ID3D11PixelShader** Graphics::getPS()
{
	return this->pShader;
}
ID3D11GeometryShader** Graphics::getGS()
{
	return this->gShader;
}
ID3D11InputLayout** Graphics::getInputL()
{
	return this->inputLayout;
}
vec2 Graphics::getWH()
{
	return vec2((float)WIDTH, (float)HEIGHT);
}


void Graphics::clearScreen()
{
	float clearColor[4] = { 0.1f,0.1f,0.1f,0 };
	immediateContext->ClearRenderTargetView(renderTarget, clearColor);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

}

void Graphics::draw(GameObject& obj)
{
	//immediateContext->PSSetShaderResources(0, 2, obj->texSRV);
	//immediateContext->VSSetConstantBuffers(0, 1, &objects[i]->getVertexConstBuffer());
	//immediateContext->PSSetConstantBuffers(0, 1, &objects[i]->getPixelConstBuffer());
	//immediateContext->IASetVertexBuffers(0, 1, &objects[i]->getVertexBuffer(), &strid, &offset);
	////this line right here
	//immediateContext->Draw((int)objects[i]->getNrOfVertex(), 0);
}

void Graphics::present()
{
	//some other stuff
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (ImGui::Begin("Light")) {
		ImGui::SliderFloat("Xpos", &light.getPos().x, 40.0f, -40.0f);
		ImGui::SliderFloat("Zpos", &light.getPos().z, 40.0f, -40.0f);
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(0, 0);
}