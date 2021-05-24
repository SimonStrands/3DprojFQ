#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	rm = new ResourceManager(gfx);
	light = new PointLight(vec3(0,0,0));
	gfx->takeIM(&this->UIManager);
	gfx->takeLight(light);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus);
	nrOfObj = 5000;
	obj = new GameObject * [nrOfObj];
	int i = 0; 
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 1000; y++) {
			obj[i] = new GameObject(rm->get_starwars(), gfx, vec3(x, 0, y), vec3(0, 0, 0), vec3(0.4, 0.4, 0.4));
			i++;
		}
	}
	UIManager.takeLight(light);
	//obj[0] = new GameObject(rm->get_IDK(), gfx, vec3(0,0,5), vec3(0,0,0), vec3(1,1,1));
	//obj[1] = new GameObject(rm->get_Stol(), gfx, vec3(2,0,0), vec3(0,0,0), vec3(1,1,1));
	//obj[2] = new GameObject(rm->get_Stol(), gfx, vec3(0, 0, 20), vec3(0, 0, 0), vec3(10, 10, 10));
	UIManager.takeObject(obj[0]);
	UIManager.takeObject(obj[1]);
	bill = new BillBoard(gfx, vec3(0.f, 1.2, 5), rm->getFire(), rm->getDef()[1], 6);
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
	delete light;
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
		gfx->updateVertexShader(*bill);
		gfx->updateGeometryShader(*bill, *camera);
		gfx->updatePixelShader(*bill);
	}
}

void Game::Update()
{
	dt.restartClock();
	//keyboard
	//obj[0]->addRot(vec3(0, 1 * dt.dt(), 0));
	//obj[1]->addRot(vec3(0, 1 * dt.dt(), 0));
	bill->update(dt.dt());
	//update
	camera->updateCamera((float)dt.dt());
	mus->UpdateMouse();

	gfx->Update((float)dt.dt());
	
}

void Game::Render()
{
	gfx->clearScreen();
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	for (int i = 0; i < nrOfObj; i++) {
		obj[i]->draw(gfx->get_IC());
	}

	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	bill->draw(gfx->get_IC());
	gfx->present();
}
