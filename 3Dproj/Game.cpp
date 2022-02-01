#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	defRend = new DeferredRendering(gfx);
	//Create a buffer for the light const buffer(hlsli)
	CreateConstBuffer(gfx, gfx->getTransGCB(), sizeof(*gfx->getLCB()), gfx->getLCB());
	//Resource manager
	rm = new ResourceManager(gfx);
	//we need mor light
	nrOfLight = 3;
	light = new Light * [nrOfLight];
	light[0] = new SpotLight(vec3(0,0,0), vec3(0,0,-1));
	light[1] = new SpotLight(vec3(0,0,0), vec3(0,0,-1));
	light[2] = new SpotLight(vec3(0,0,0), vec3(0,0,-1));
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx);

	gfx->takeIM(&this->UIManager);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0));

	//////OBJECTS///////////
	nrOfObj = 3;
	obj = new GameObject * [nrOfObj];
	//OBJECTS
	obj[0] = new GameObject(rm->get_Ball(), gfx, vec3(0.f, 5.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(0.1f, 0.1f, 0.1f));//light
	obj[1] = new GameObject(rm->get_Stol(), gfx, vec3(0.f, 0.f, 10.f), vec3(-1.56f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	obj[1]->changePoint(vec3(0, 0, 2));
	obj[2] = new GameObject(rm->get_IDK(), gfx, vec3(0.f, 0.f, 20.f), vec3(-1.6f, -1.6f, 3.2f), vec3(20.f, 20.f, 20.f));
	//obj[3] = new GameObject(rm->get_Stol(), gfx, vec3(-15.f, 0.f, 15.f), vec3(0.f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	bill = new BillBoard(gfx, vec3(0.f, 0.f, 9.f), rm->getFire(), rm->getDef()[1], 6);
	/////LIGHT////////
	LightVisualizers = new GameObject * [nrOfLight];
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers[i] = new GameObject(rm->get_Ball(), gfx, light[i]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.1f, 0.1f, 0.1f));
	}


	//UI
	UIManager.takeLight(light[0]);
	//UIManager.takeLight(light[1]);
	UIManager.takeObject(obj[1]);
	UIManager.takeObject(obj[2]);
	//UIManager.takeObject(obj[3]);
	
	gfx->takeLight((SpotLight**)light, nrOfLight);

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
		shadowMap->setUpdateShadow();
		//måste uppdatera detta så inte hela object uppdateras när bara skugga ska
		for (int i = 0; i < nrOfLight; i++) {
			shadowMap->inUpdateShadow(i);
			for (int i = 0; i < nrOfObj; i++) {
				obj[i]->updateVertexShader(gfx);
			}
			bill->UpdateShader(gfx, camera->getPos());
			DrawAllShadowObject();
		}

		
		//for shadow

		Update();
		for (int i = 0; i < nrOfObj; i++) {
			obj[i]->updateVertexShader(gfx);
			obj[i]->updatePixelShader(gfx);
		}
		bill->UpdateShader(gfx, camera->getPos());

		//
		defRend->BindFirstPass();
		//gfx->setRenderTarget();
		this->DrawToBuffer();

		defRend->BindSecondPass(shadowMap->GetshadowResV());

		gfx->setRenderTarget();
		this->ForwardDraw();
		gfx->present();
	}
}

void Game::Update()
{
	dt.restartClock();
	//obj[0]->changePos(this->light->getPos());
	//keyboard
	//obj[0]->addRot(vec3(0, 1 * dt.dt(), 0));
	obj[1]->addRot(vec3(1.f*dt.dt(), 1.1f * dt.dt(), 1.2f*dt.dt()));
	//obj[2]->addRot(vec3(0, 0.7f * (float)dt.dt(), 0));
	//snake->addRot(vec3(0, 0.7f * (float)dt.dt(), 0));
	camera->updateCamera((float)dt.dt());
	bill->update((float)dt.dt());
	mus->UpdateMouse();
	
	gfx->Update((float)dt.dt());
	
	if (getkey('C')) {
		camera->setPosition(light[0]->getPos());
	}
}

void Game::DrawToBuffer()
{
	gfx->get_IC()->PSSetShaderResources(4, 1, &shadowMap->GetshadowResV());
	
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->PSSetShader(defRend->getPS(), nullptr, 0);
	for (int i = 0; i < nrOfObj; i++) {
		//obj[i]->draw(gfx->get_IC());
		obj[i]->drawDefTest(gfx->get_IC());
	}
	
	
}

void Game::ForwardDraw()
{
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
		obj[i]->draw(gfx->get_IC(), true);
	}
	bill->draw(gfx->get_IC());
}
