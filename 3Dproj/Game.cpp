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
	nrOfLight = 2;
	light = new Light * [nrOfLight];
	light[0] = new SpotLight(vec3(0,0,0), vec3(0,0,-1));
	light[1] = new SpotLight(vec3(0,0,20), vec3(-2.8,0,-1));
	//
	//light[2] = new SpotLight(vec3(0,0,0), vec3(0,0,-1));
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx);

	gfx->takeIM(&this->UIManager);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0));

	//////OBJECTS///////////
	nrOfObj = 4;
	obj = new GameObject * [nrOfObj];
	//OBJECTS
	obj[0] = new GameObject(rm->get_Ball(), gfx, vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	obj[1] = new GameObject(rm->get_Stol(), gfx, vec3(10.f, 5.f, 10.f), vec3(-1.56f, 1.56f, 3.2f), vec3(1.f, 1.f, 1.f));
	obj[2] = new GameObject(rm->get_IDK(), gfx, vec3(0.f, 5.f, 20.f), vec3(-1.6f, -1.6f, 3.2f), vec3(20.f, 20.f, 20.f));
	obj[3] = new GameObject(rm->get_Ball(), gfx, vec3(-5.f, 10.f, 10.f), vec3(0.f, 0.f, 1.6f), vec3(1.f, 1.f, 1.f));
	//obj[4] = new GameObject(rm->get_Models("stormtrooper.obj"), gfx, vec3(-15.f, 0.f, 15.f), vec3(0.f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	obj[0]->setTesselation(true, gfx);


	bill = new BillBoard(gfx, vec3(0.f, 0.f, 9.f), rm->getFire(), rm->getDef()[1], 6);
	//DCube = new DynamicCube(rm->get_Stol(), gfx, vec3(-10.f, 0.f, 10.f), vec3(1.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	/////LIGHT////////
	LightVisualizers = new GameObject * [nrOfLight];
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers[i] = new GameObject(rm->get_Models("stormtrooper.obj"), gfx, light[i]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.1f, 0.1f, 0.1f));
		//LightVisualizers[i]->setTesselation(false, gfx);
	}


	//UI
	for (int i = 0; i < nrOfLight; i++) {
		UIManager.takeLight(light[i]);
	}
	UIManager.takeObject(obj[0]);
	//UIManager.takeObject(obj[1]);
	//UIManager.takeObject(obj[2]);
	
	
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
		gfx->setTransparant(false);
		//for shadow
		shadowMap->setUpdateShadow();
		//m�ste uppdatera detta s� inte hela object uppdateras n�r bara skugga ska
		for (int i = 0; i < nrOfLight; i++) {
			shadowMap->inUpdateShadow(i);
			for (int i = 0; i < nrOfObj; i++) {
				obj[i]->updateVertexShader(gfx);
			}
			//DCube->updateVertexShader(gfx);//
			bill->UpdateShader(gfx, camera->getPos());
			DrawAllShadowObject();
		}


		Update();
		for (int i = 0; i < nrOfObj; i++) {
			obj[i]->updateVertexShader(gfx);
			obj[i]->updatePixelShader(gfx);
		}
		//DCube->updateVertexShader(gfx);
		//DCube->updatePixelShader(gfx);
		for (int i = 0; i < nrOfLight; i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
			LightVisualizers[i]->updateVertexShader(gfx);
		}
		bill->UpdateShader(gfx, camera->getPos());

		//
		defRend->BindFirstPass();

		this->DrawToBuffer();
		//DrawDynamicCube();

		defRend->BindSecondPass(shadowMap->GetshadowResV());

		gfx->setTransparant(true);
		gfx->setRenderTarget();
		this->ForwardDraw();
		gfx->present();
	}
	printf("quit");
}

void Game::Update()
{
	dt.restartClock();
	//obj[0]->changePos(this->light->getPos());
	//keyboard
	//obj[0]->addRot(vec3(0, 1 * dt.dt(), 0));
	//obj[1]->addRot(vec3(1.f*dt.dt(), 1.1f * dt.dt(), 1.2f*dt.dt()));
	//obj[2]->addRot(vec3(0, 0.7f * (float)dt.dt(), 0));
	camera->updateCamera((float)dt.dt());
	bill->update((float)dt.dt());
	mus->UpdateMouse();
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers[i]->changePos(light[i]->getPos());
	}
	
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
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < nrOfObj; i++) {
		obj[i]->draw(gfx);
	}

	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers[i]->drawDefTest(gfx->get_IC());
	}
	
	
}

void Game::DrawDynamicCube()
{
	for (int i = 0; i < 6; i++) {
		//defRend->BindFirstPass();
		DCube->draw(gfx->get_IC());
		//change camera angel 2
		defRend->BindSecondPassFunc(shadowMap->GetshadowResV(), DCube->getUAVs()[i]);
	}
}

void Game::ForwardDraw()
{
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	bill->draw(gfx);
}

void Game::DrawAllShadowObject()
{
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < nrOfObj; i++) {
		obj[i]->draw(gfx, true);
	}
}
