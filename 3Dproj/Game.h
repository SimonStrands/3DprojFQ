#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "deltaTime.h"
#include "Mouse.h"
#include "Camera.h"
#include "imguiManager.h"
#include "ResourceManager.h"
#include "Light.h"

//git
class Game {
public:
	Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Game();
	void run();
	void DrawToBuffer();
	void DrawAllShadowObject();//without any otherShaders
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
	PointLight *light;
	GameObject** obj;
	BillBoard *bill;
	Mouse* mus;

	//var
	int nrOfObj;
};