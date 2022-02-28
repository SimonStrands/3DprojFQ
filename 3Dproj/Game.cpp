#include "Game.h"
//git
Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	defRend = new DeferredRendering(gfx);
	//Create a buffer for the light const buffer(hlsli)
	CreateConstBuffer(gfx, gfx->getConstBuffers(0), sizeof(*gfx->getLCB()), gfx->getLCB());
	CreateConstBuffer(gfx, gfx->getConstBuffers(1), sizeof(*gfx->getCPCB()), gfx->getCPCB());
	//Resource manager
	rm = new ResourceManager(gfx);
	//we need mor light
	nrOfLight = 2;
	gfx->getLCB()->nrOfLights.element = nrOfLight;
	light = new Light * [nrOfLight];
	light[0] = new DirLight(vec3(0,0,0), vec3(0, 0, -1));
	light[1] = new SpotLight(vec3(1, 0.2f, 0), vec3(0.1f, 0, -1));
	//light[2] = new SpotLight(vec3(10,0,0), vec3(0.f,0.f,-1.f));
	//light[3] = new SpotLight(vec3(-10,0,0), vec3(0,0,-1));

	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx);

	gfx->takeIM(&this->UIManager);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0));

	//////OBJECTS///////////
	nrOfObj = 6;
	obj = new GameObject * [nrOfObj];
	//OBJECTS
	obj[0] = new GameObject(rm->get_Ball(), gfx, vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	obj[1] = new GameObject(rm->get_Stol(), gfx, vec3(10.f, 5.f, 10.f), vec3(-1.56f, 1.56f, 3.2f), vec3(1.f, 1.f, 1.f));
	//walls
	obj[2] = new GameObject(rm->get_IDK(), gfx, vec3(0.f, 5.f, 20.f), vec3(-1.6f, -1.6f, 3.2f), vec3(20.f, 20.f, 20.f));
	obj[3] = new GameObject(rm->get_IDK(), gfx, vec3(20.f, 5.f, 0.f), vec3(-1.6f, 0.f, 3.2f), vec3(20.f, 20.f, 20.f));
	obj[4] = new GameObject(rm->get_IDK(), gfx, vec3(0.f, 5.f, -20.f), vec3(-1.6f, 1.6f, 3.2f), vec3(20.f, 20.f, 20.f));
	obj[5] = new GameObject(rm->get_IDK(), gfx, vec3(-20.f, 5.f, 0.f), vec3(-1.6f, 3.f, 3.2f), vec3(20.f, 20.f, 20.f));

	//obj[6] = new GameObject(rm->get_Models("stormtrooper.obj"), gfx, vec3(5.f, 0.f, 0.f), vec3(0.f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	//obj[7] = new GameObject(rm->get_Models("stormtrooper.obj"), gfx, vec3(0.f, 0.f, 5.f), vec3(1.6f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	//obj[8] = new GameObject(rm->get_Models("stormtrooper.obj"), gfx, vec3(-5.f, 0.f, 0.f), vec3(3.2f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	//obj[9] = new GameObject(rm->get_Models("stormtrooper.obj"), gfx, vec3(0.f, 0.f, -5.f), vec3(-1.6f, 1.56f, 0.f), vec3(1.f, 1.f, 1.f));
	obj[0]->setTesselation(false, gfx);

	bill = new BillBoard(gfx, vec3(0.f, 0.f, 9.f), rm->getFire(), rm->getDef()[1], 6);
	billManager = new BillBoardManager(gfx, 10);

	//DCube cannot use standard obj:s without fucking others shaders
	DCube = new DynamicCube(rm->get_Models("roundsol.obj"), gfx, vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	//DCube = new DynamicCube(rm->get_Models("DCube.obj"), gfx, vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	/////LIGHT////////
	LightVisualizers = new GameObject * [nrOfLight];
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers[i] = new GameObject(rm->get_Models("roundsol.obj"), gfx, light[i]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.1f, 0.1f, 0.1f));
	}


	//UI
	for (int i = 0; i < nrOfLight; i++) {
		UIManager.takeLight(light[i]);
	}
	//UIManager.takeObject(DCube);
	UIManager.takeObject(obj[0]);
	
	
	gfx->takeLight((SpotLight**)light, nrOfLight);

	currentTime = 0;
	lightNr = 0;
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
	delete billManager;
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
		//måste uppdatera detta så inte hela object uppdateras när bara skugga ska
		for (int i = 0; i < nrOfLight; i++) {
			shadowMap->inUpdateShadow(i);
			updateShaders(true, false);
			bill->UpdateShader(gfx, camera->getPos());
			billManager->update(dt.dt(),gfx);
			DrawAllShadowObject();
		}
		gfx->Projection(0);//last can be dir light


		Update();
		updateShaders();
		bill->UpdateShader(gfx, camera->getPos());
		DrawDynamicCube();//cant be before bindfirstPass();
		//
		defRend->BindFirstPass();

		this->DrawToBuffer();

		defRend->BindSecondPass(shadowMap->GetshadowResV());

		gfx->setTransparant(true);
		gfx->setRenderTarget();
		this->ForwardDraw();
		gfx->present(this->lightNr);
	}
	printf("quit");
}

void Game::Update()
{
	dt.restartClock();
	//obj[0]->changePos(this->light->getPos());
	//keyboard
	obj[0]->addRot(vec3(0, 1.f * dt.dt(), 0));
	//obj[1]->addRot(vec3(1.f*dt.dt(), 1.1f * dt.dt(), 1.2f*dt.dt()));
	//obj[2]->addRot(vec3(0, 0.7f * (float)dt.dt(), 0));
	camera->updateCamera((float)dt.dt());
	bill->update((float)dt.dt());
	mus->UpdateMouse();
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers[i]->changePos(light[i]->getPos());
	}
	
	gfx->Update((float)dt.dt(), camera->getPos());

	if (getkey('C')) {
		camera->setPosition(light[lightNr]->getPos());
		camera->setRotation(light[lightNr]->getRotation());
	}
	if (getkey('1')) {
		lightNr = 0;
	}
	if (getkey('2')) {
		lightNr = 1;
	}
	if (getkey('3')) {
		lightNr = 2;
	}
	if (getkey('4')) {
		lightNr = 3;
	}
	if (getkey('V')) {
		camera->setPosition(vec3(0, 0, 15));
		camera->setRotation(vec3(3.2f, 0, 0));
		light[0]->getPos().x = 0;
		light[0]->getPos().y = 0;
		light[0]->getPos().z = 15;
		light[0]->getRotation().x = 3.2f;
		light[0]->getRotation().y = 0;
		light[0]->getRotation().z = 0;
	}
	if (getkey('B')) {
		camera->setPosition(vec3(0, 0, 0));
		camera->setRotation(vec3(0, 0, 0));
		light[0]->getPos().x = 0;
		light[0]->getPos().y = 0;
		light[0]->getPos().z = 0;
		light[0]->getRotation().x = 0;
		light[0]->getRotation().y = 0;
		light[0]->getRotation().z = 0;
	}
}

void Game::DrawToBuffer()
{	
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

void Game::DrawToBufferDebug()
{
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < nrOfObj; i++) {
		obj[i]->drawDebug(gfx);
	}

	//gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	//gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	//gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	//gfx->get_IC()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	//gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//for (int i = 0; i < nrOfLight; i++) {
	//	LightVisualizers[i]->drawDefTest(gfx->get_IC());
	//}
}

void Game::DrawDynamicCube()
{
	gfx->Projection(2);
	vec3 camLP = camera->getPos();
	vec3 camRT = camera->getRot();
	vec3 DCRot = DCube->getRot();
	camera->setPosition(vec3(DCube->getPos()));
	camera->setRotation(vec3(DCube->getRot()));

	//draw all textures
	for (int i = 0; i < 6; i++) {
		//change camera angel 2
		switch (i)
		{
		case 0:
			camera->setRotation(vec3(DCRot.x + 1.6f, DCRot.y, 0));//right
			break;
		case 1:
			camera->setRotation(vec3(DCRot.x - 1.6f, DCRot.y, 0));//left
			break;
		case 2:
			camera->setRotation(vec3(DCRot.x, DCRot.y+1.6f, 0));//up
			break;
		case 3:
			camera->setRotation(vec3(DCRot.x, DCRot.y - 1.6f, 0));//bottom
			break;
		case 4:
			camera->setRotation(vec3(DCRot.x, DCRot.y, 0));//forward
			break;
		case 5:
			camera->setRotation(vec3(DCRot.x + 3.2f, DCRot.y, 0));//back
			break;
		}
		camera->updateCamera();
		updateShaders(true, false);
		defRend->BindFirstPass();//needed
		this->DrawToBufferDebug();
		defRend->BindSecondPassFunc(shadowMap->GetshadowResV(), DCube->getUAVs()[i],16,40, DCube->getCSShader());
	}


	//draw the cube
	gfx->Projection(0);
	camera->setPosition(camLP);
	camera->setRotation(camRT);
	camera->updateCamera();
	updateShaders(true, false);
	
}

void Game::ForwardDraw()
{
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	DCube->update(camera->getPos(), gfx);
	DCube->draw(gfx->get_IC());

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

void Game::updateShaders(bool vs, bool ps)
{
	if (vs)
	{
		DCube->updateVertexShader(gfx);
		for (int i = 0; i < nrOfObj; i++) {
			obj[i]->updateVertexShader(gfx);
		}
		for (int i = 0; i < nrOfLight; i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
	}
	if (ps) {
		DCube->updatePixelShader(gfx);
		for (int i = 0; i < nrOfObj; i++) {
			obj[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < nrOfLight; i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
		}
	}
}
