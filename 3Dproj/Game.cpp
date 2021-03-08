#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	rm = new ResourceManager(gfx);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus);
	nrOfObj = 2;
	obj = new GameObject * [nrOfObj];
	obj[0] = new GameObject(rm->get_IDK(), gfx, vec3(0,0,5), vec3(0,0,0), vec3(1,1,1));
	obj[1] = new GameObject(rm->get_IDK(), gfx, vec3(5,0,5), vec3(0,0,0), vec3(1,1,1));
	bill = new BillBoard(gfx, vec3(-5, 0, 5), rm->getFire());
}

Game::~Game()
{
	delete gfx;
	delete mus;
	delete camera;
	delete rm;
	for (int i = 0; i < nrOfObj; i++) {
		delete obj[i];
	}
	delete[] obj;
	delete bill;
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
		Render();
		for (int i = 0; i < nrOfObj; i++) {
			gfx->updateVertexShader(*obj[i]);
			gfx->updatePixelShader(*obj[i]);
		}
	}
}

void Game::Update()
{
	dt.restartClock();
	//keyboard
	obj[0]->addRot(vec3(0, 1 * dt.dt(), 0));
	//update
	camera->updateCamera((float)dt.dt());
	mus->UpdateMouse();

	gfx->Update((float)dt.dt());
	
}

void Game::Render()
{
	gfx->clearScreen();
	gfx->get_IC()->VSSetShader(gfx->getVS(), nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS(), nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS(), nullptr, 0);

	for (int i = 0; i < nrOfObj; i++) {
		obj[i]->draw(gfx->get_IC());
	}
	

	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	bill->draw(gfx->get_IC());

	gfx->present();
}
