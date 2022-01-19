#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "deltaTime.h"
#include "Mouse.h"
#include "Camera.h"
#include "imguiManager.h"
#include "ResourceManager.h"

#include "AnimationModel.h"
#include "GameObject.h"
#include "BillBoard.h"
#include "Light.h"

#include "ShadowMap.h"
#include "Graphics.h"

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
	Mouse* mus;
	Camera* camera;
	ShadowMap* shadowMap;

	//game objects
	PointLight *light;
	GameObject** obj;
	BillBoard *bill;
	AnimationModel* snake;

	//var
	int nrOfObj;
	float currentTime;
};