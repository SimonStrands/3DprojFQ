#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	camera = new Camera(gfx);
	UINT WIDTH = 800;
	UINT HEIGHT = 600;
	//if (!setUpWindow(hInstance, WIDTH, HEIGHT, nCmdShow, wnd)) {
	//	std::cerr << "failed" << std::endl;
	//}


	nrOfObj = 4;
	obj = new object * [nrOfObj];
	obj[0] = new object("obj/objtest.obj", *gfx, "", vec3(0,0,10), vec3(0,0,0));
	obj[1] = new object("obj/stol.obj", *gfx, "babyyoda.jpg", vec3(10,0,0),vec3(0,0,0));
	obj[2] = new object("obj/stol.obj", *gfx, "", vec3(0,0,-10),vec3(0,0,0));
	obj[3] = new object("obj/stol.obj", *gfx, "babyyoda.jpg", vec3(-10,0,0),vec3(0,0,0));
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
		for (int i = 0; i < nrOfObj; i++) {
			gfx->updateWorldMatrix(*obj[i]);
		}
	}
}

void Game::Update()
{
	dt.restartClock();
	//keyboard
	
	//update
	camera->updateCamera(dt.dt());
	//render
	gfx->Update(dt.dt());
}
