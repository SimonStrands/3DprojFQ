#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	rm = new ResourceManager(gfx);
	light = new PointLight(vec3(0,0,0), vec3(0,0,-1));
	gfx->takeIM(&this->UIManager);
	gfx->setGame(this);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0));
	nrOfObj = 4;
	obj = new GameObject * [nrOfObj];

	//OBJECTS
	obj[0] = new GameObject(rm->get_Ball(), gfx, light->getPos(), vec3(0, 0 ,0), vec3(0.1, 0.1, 0.1));//light
	obj[1] = new GameObject(rm->get_IDK(), gfx, vec3(0, 0, 20), vec3(-1.56, 1.56,0), vec3(10, 10, 10));
	obj[2] = new GameObject(rm->get_Ball(), gfx, vec3(3, 0, 10), vec3(-1.6, -1.6, 3.2), vec3(1, 1, 1));
	obj[3] = new GameObject(rm->get_Stol(), gfx, vec3(-15, 0, 15), vec3(0, 1.56, 0), vec3(1, 1, 1));
	bill = new BillBoard(gfx, vec3(0.f, 1.2f, 5), rm->getFire(), rm->getDef()[1], 6);

	//UI
	UIManager.takeLight(light);
	UIManager.takeObject(obj[1]);
	UIManager.takeObject(obj[2]);
	UIManager.takeObject(obj[3]);

	gfx->takeLight(light); 

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
		gfx->clearScreen();
		
		gfx->updateVertexShader(*bill);
		gfx->updateGeometryShader(*bill, *camera);
		gfx->updatePixelShader(*bill);

		gfx->drawShadowBuffer();
		for (int i = 0; i < nrOfObj; i++) {
			gfx->updateVertexShader(*obj[i]);
		}
		DrawAllShadowObject();

		
		Update();
		for (int i = 0; i < nrOfObj; i++) {
			gfx->updateVertexShader(*obj[i]);
			gfx->updatePixelShader(*obj[i]);
		}

		gfx->drawToBuffer();
		gfx->present();
	}
	
}

void Game::Update()
{
	dt.restartClock();
	obj[0]->changePos(this->light->getPos());
	//keyboard
	//obj[0]->addRot(vec3(0, 1 * dt.dt(), 0));
	//obj[1]->addRot(vec3(0, 1.5 * dt.dt(), 0));
	//obj[2]->addRot(vec3(0, 0.7 * dt.dt(), 0));
	camera->updateCamera((float)dt.dt());
	bill->update((float)dt.dt());
	mus->UpdateMouse();

	gfx->Update((float)dt.dt());

	if (getkey('C')) {
		camera->setPosition(light->getPos());
	}
	
}

void Game::DrawToBuffer()
{
	gfx->get_IC()->PSSetShaderResources(3, 1, &gfx->getShadowMap()->fromDepthToSRV());

	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	for (int i = 1; i < nrOfObj; i++) {
		obj[i]->draw(gfx->get_IC());
	}

	//gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	//gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	//gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	//bill->draw(gfx->get_IC());

}

void Game::DrawAllShadowObject()
{
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	for (int i = 1; i < nrOfObj; i++) {
		obj[i]->draw(gfx->get_IC());
	}
}
