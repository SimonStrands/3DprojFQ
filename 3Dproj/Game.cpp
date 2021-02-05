#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	camera = new Camera(gfx);

	nrOfObj = 2;
	obj = new object * [nrOfObj];
	obj[0] = new object("obj/objtest.obj", *gfx, vec3(0,0,0));
	obj[1] = new object("obj/stol.obj", *gfx,vec3(0,0,0),vec3(2,3,4));
	gfx->createBuffer();
	gfx->setObjects(obj, nrOfObj);
	
}

Game::~Game()
{
	delete gfx;
	delete camera;
	for (int i = 0; i < nrOfObj; i++) {
		delete obj[i];
	}
	delete[] obj;
}

void Game::run()
{
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Update();
		for (int i = 0; i < 1; i++) {
			gfx->updateWorldMatrix(*obj[i]);
		}
		
	}
}

void Game::Update()
{
	dt.restartClock();
	//keyboard
	
	//update
	camera->updateCamera(dt);
	//render
	gfx->Update(dt);
}
