#include "Graphics.h"
#include "imguiManager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

//debug nothing to look here at
void Graphics::debugcd()
{
}
//keyboard buttons + rotation
void Graphics::keyboardDebug()
{

}


void Graphics::Projection()
{
	//setting projection matrix
	vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
}

Graphics::Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) :
	speed(1.5f)
{
	fov = 90.f;
	ratio = 16.f / 9.f;
	farPlane = 200.f;
	nearPlane = 0.1f;
	nrOfObject = 0;
	Pg_pConstantBuffer = nullptr;
	normalMapping = true;//?
	inputLayout = new ID3D11InputLayout * [3]{nullptr, nullptr, nullptr};
	vShader = new ID3D11VertexShader * [3]{ nullptr, nullptr, nullptr };
	gShader = new ID3D11GeometryShader * [2]{ nullptr, nullptr };
	pShader = new ID3D11PixelShader * [3] { nullptr, nullptr,nullptr };
	//setting normal value for pcbd
	//this->LCBG.lightPos = { 1,1,1,1 };
	this->LCBG.lightColor = { 1,1,1,0 };
	this->LCBG.cameraPos = { 0,0,1,1 };
	this->pcbd.ka = { 0.5f,0.5f,0.5f,1 };
	this->pcbd.kd = { 1.f,1.f,1.f,0 };
	this->pcbd.ks = {1.f,1.f,1.f,0};
	//
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
	immediateContext->PSSetShader(getPS()[0], nullptr, 0);
	immediateContext->RSSetViewports(1, &viewPort);
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	immediateContext->RSSetState(pRS);
	ImGui_ImplDX11_Init(device, immediateContext);
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	shutDownWindow();
	
	if (gShader[0] != nullptr) {
		gShader[0]->Release();
	}
	for (int i = 0; i < 2; i++) {
		if (vShader[i] != nullptr) {
			vShader[i]->Release();
		}
		if (pShader[i] != nullptr) {
			pShader[i]->Release();
		}
		if (inputLayout[i] != nullptr) {
			inputLayout[i]->Release();
		}
	}
	if (pShader != nullptr) {
		delete pShader;
	}
	if (gShader != nullptr) {
		delete gShader;
	}
	if (vShader != nullptr) {
		delete vShader;
	}
	if (inputLayout != nullptr) {
		delete inputLayout;
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
	if (bs != nullptr) {
		bs->Release();
	}
}

float nextFpsUpdate = 0;
void Graphics::Update(float dt)
{
	//don't have light pos but light view
	//LCBG.lightPos.element[0] = this->getLight()->getPos().x;
	//LCBG.lightPos.element[1] = this->getLight()->getPos().y;
	//LCBG.lightPos.element[2] = this->getLight()->getPos().z;
	//LCBG.lightPos.element[3] = 1;

	if (getkey('N')) {
		LCBG.cameraPos.element[3] = 1;
		//LCBG.lightPos.element[3] = 3;
	}
	if (getkey('M')) {
		LCBG.cameraPos.element[3] = 0;
		//LCBG.lightPos.element[3] = 3;
	}
	LCBG.projection.element = vcbd.projection.element;
	for (int i = 0; i < nrOfLights; i++) {
		LCBG.lightView.element[i] = this->light[i]->getLightView();
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	immediateContext->Map(this->Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &LCBG, sizeof(LCBGS));
	immediateContext->Unmap(this->Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	this->immediateContext->PSSetConstantBuffers(3, 1, &this->Pg_pConstantBuffer);
	this->immediateContext->CSSetConstantBuffers(6, 1, &this->Pg_pConstantBuffer);
	//fps
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
Gcb* Graphics::getGcb()
{
	return &gcbd;
}
LCBGS* Graphics::getLCB()
{
	return &LCBG;
}
void Graphics::setVView(DirectX::XMMATRIX &mat)
{
	mat = this->vcbd.view.element;
}
void Graphics::setVProj(DirectX::XMMATRIX& mat)
{
	mat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
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
IDXGISwapChain*& Graphics::getSwapChain()
{
	return this->swapChain;
}
ID3D11InputLayout** Graphics::getInputL()
{
	return this->inputLayout;
}
ID3D11RenderTargetView*& Graphics::getRenderTarget()
{
	return this->renderTarget;
}
ID3D11DepthStencilView* Graphics::getDepthStencil()
{
	return this->dsView;
}
ID3D11Buffer*& Graphics::getTransGCB()
{
	return this->Pg_pConstantBuffer;
}
SpotLight **Graphics::getLight()
{
	return this->light;
}
vec2 Graphics::getWH()
{
	return vec2((float)WIDTH, (float)HEIGHT);
}

void Graphics::takeLight(SpotLight** light, int nrOfLights)
{
	this->nrOfLights = nrOfLights;
	this->light = light;
}

void Graphics::takeIM(ImguiManager* manager)
{
	this->imguimanager = manager;
}

void Graphics::clearScreen()
{
	float clearColor[4] = { 0.1f,0.1f,0.1f,0 };
	immediateContext->ClearRenderTargetView(renderTarget, clearColor);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void Graphics::setRenderTarget()
{
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
}

void Graphics::present()
{
	this->imguimanager->updateRender();
	swapChain->Present(0, 0);
}
