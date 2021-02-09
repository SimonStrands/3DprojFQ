#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "deltaTime.h"
#include "Camera.h"
//git
class Game {
public:
	Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Game();
	void run();
private:
	//width and height of window
	MSG msg = {};
	Graphics *gfx;
	void Update();
private:
	//logic
	DeltaTime dt;

	//game objects
	Camera *camera;
	object** obj;
	Mouse* mus;

	//var
	int nrOfObj;
};