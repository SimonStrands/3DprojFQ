#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	rm = new ResourceManager(gfx);
	light = new PointLight(vec3(0,0,0), vec3(0,0,-1));
	this->shadowMap = new ShadowMap(light, gfx);
	gfx->takeIM(&this->UIManager);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0));
	nrOfObj = 4;
	obj = new GameObject * [nrOfObj];
	//snake = new Model("obj/stol.obj", gfx, vec3(0,0,10),vec3(0,0,0), vec3(1,1,1));
	//snake = new Model("obj/SnakeTest.fbx", gfx, vec3(-5,0,10),vec3(0,0,0), vec3(1,1,1));
	snake = new AnimationModel("obj/SnakeTest.fbx", gfx, vec3(0, 0, 10), vec3(90, 0, 0), vec3(1, 1, 1));
	//OBJECTS
	obj[0] = new GameObject(rm->get_Ball(), gfx, light->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.1f, 0.1f, 0.1f));//light
	obj[1] = new GameObject(rm->get_IDK(), gfx, vec3(0.f, 0.f, 20.f), vec3(-1.56f, 1.56f, 0.f), vec3(20.f, 20.f, 20.f));
	obj[2] = new GameObject(rm->get_Ball(), gfx, vec3(3.f, 0.f, 10.f), vec3(-1.6f, -1.6f, 3.2f), vec3(1.f, 1.f, 1.f));
	obj[3] = new GameObject(rm->get_Stol(), gfx, vec3(-15.f, 0.f, 15.f), vec3(0.f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	bill = new BillBoard(gfx, vec3(0.f, 0.f, 10.f), rm->getFire(), rm->getDef()[1], 6);

	//UI
	UIManager.takeLight(light);
	UIManager.takeObject(obj[1]);
	UIManager.takeObject(obj[2]);
	UIManager.takeObject(obj[3]);
	
	gfx->takeLight(light); 

	currentTime = 0;
}

Game::~Game() 
{
	delete gfx;
	delete rm;
	delete camera;
	delete light;
	for (int i = 0; i < nrOfObj; i++) {
		delete obj[i];
	}
	delete[] obj;
	delete bill;
	delete mus;
	if (shadowMap != nullptr) {
		delete shadowMap;
	}
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

		//for shadow
		shadowMap->DrawShadowBuffer(gfx);
		for (int i = 0; i < nrOfObj; i++) {
			obj[i]->updateVertexShader(gfx);
		}
		bill->UpdateShader(gfx, camera->getPos());
		DrawAllShadowObject();
		//for shadow

		Update();
		for (int i = 0; i < nrOfObj; i++) {
			obj[i]->updateVertexShader(gfx);
			obj[i]->updatePixelShader(gfx);
		}
		bill->UpdateShader(gfx, camera->getPos());
		//
		this->snake->update(dt.dt());
		snake->updatePixelShader(gfx);
		//
		gfx->setRenderTarget();
		this->DrawToBuffer();
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
	obj[2]->addRot(vec3(0, 0.7f * (float)dt.dt(), 0));
	//snake->addRot(vec3(0, 0.7f * (float)dt.dt(), 0));
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
	gfx->get_IC()->PSSetShaderResources(3, 1, &shadowMap->GetshadowResV());
	
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < nrOfObj; i++) {
		obj[i]->draw(gfx->get_IC());
	}

	gfx->get_IC()->PSSetShaderResources(0, 1, rm->getDef());
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[2]);
	gfx->get_IC()->VSSetShader(gfx->getVS()[2], nullptr, 0);
	snake->draw(gfx->get_IC());//doesn't need ctx but I fuck up
	
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	bill->draw(gfx->get_IC());
}

void Game::DrawAllShadowObject()
{
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	for (int i = 1; i < nrOfObj; i++) {
		obj[i]->draw(gfx->get_IC());
	}
	//snake->draw(gfx->get_IC());
	bill->draw(gfx->get_IC());
}
