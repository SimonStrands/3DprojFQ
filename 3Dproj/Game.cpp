#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	camera = new Camera(gfx);
}

Game::~Game()
{
	delete gfx;
	delete camera;
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
