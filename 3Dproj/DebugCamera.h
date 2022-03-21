#pragma once
#include "Graphics.h"
#include "Camera.h"

class DebugCamera {
public:
	DebugCamera(Graphics*& gfx, _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
private:
	const UINT WIDTH = 300;
	const UINT HEIGHT = 300;
private:
	Camera* cam;
	HWND wnd;
	D3D11_VIEWPORT viewPort;
	ID3D11RenderTargetView* RTV;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
};