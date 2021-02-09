#pragma once
#include <Windows.h>
#include "Mouse.h"
#include <vector>
//git
class Window {
public:
	bool setUpWindow(HINSTANCE hInstance, UINT WIDTH, UINT HEIGHT, int nCmdShow, HWND& wnd);
	LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	std::vector<BYTE> rawBuffer;
	mouse mus;
};

