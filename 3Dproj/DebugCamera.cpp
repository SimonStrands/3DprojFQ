#include "DebugCamera.h"

DebugCamera::DebugCamera(Graphics*& gfx, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	if (!setUpWindow(hInstance, WIDTH, HEIGHT, nCmdShow, wnd)) {
		std::cerr << "failed" << std::endl;
	}
	ShowWindow(wnd, nCmdShow);
	CreateInsterface(this->WIDTH, this->HEIGHT, wnd, device, immediateContext, this->swapChain);
	CreateRenderTargetView(device, swapChain, RTV);
	SetViewport(viewPort, this->WIDTH, this->HEIGHT);
}
