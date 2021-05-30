#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "deltaTime.h"
#include "Mouse.h"
#include "Camera.h"
#include "imguiManager.h"
#include "ResourceManager.h"
#include "Light.h"

#include "GameObject.h"
#include "BillBoard.h"

//git
class Game {
public:
	Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Game();
	void run();
	void DrawToBuffer(bool ps = true);
private:
	//width and height of window
	MSG msg = {};
	Graphics *gfx;
	ResourceManager* rm;
	void Update();
private:
	//logic and others
	DeltaTime dt;
	ImguiManager UIManager;

	//game objects
	Camera *camera;
	Light *light;
	GameObject** obj;
	BillBoard *bill;
	Mouse* mus;

	//var
	int nrOfObj;
};