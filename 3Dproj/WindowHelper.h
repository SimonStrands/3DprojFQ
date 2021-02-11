#pragma once
#include <Windows.h>
#include "Mouse.h"
#include <vector>
//git
bool setUpWindow(HINSTANCE hInstance, UINT WIDTH, UINT HEIGHT, int nCmdShow, HWND& wnd);
LRESULT WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
class Window {
public:
	
private:
	std::vector<BYTE> rawBuffer;
	mouse mus;
};

