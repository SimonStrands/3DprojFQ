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

	nrOfLight = 2;
	gfx->getLCB()->nrOfLights.element = nrOfLight;
	light = new Light * [nrOfLight];
	light[0] = new DirLight(vec3(54,11,8), vec3(0.1, -1.58, 1));
	light[1] = new SpotLight(vec3(14, 46, 8), vec3(0.1, -1.58, 1));
	//light[1] = new SpotLight(vec3(25, 0.2f, 0), vec3(1, 1, 0));
	//light[2] = new SpotLight(vec3(-25, 0.2f, 0), vec3(0, -1, 0));
	//light[3] = new SpotLight(vec3(30, 50, 0), vec3(-1, -1, 0));
	//light[4] = new SpotLight(vec3(1, 0.2f, 0), vec3(0.1f, 0, -1));
	
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx);
	
	gfx->takeIM(&this->UIManager);
	mus = new Mouse(gfx->getWH());
	camera = new Camera(gfx, mus, vec3(0,0,0),vec3(0,0,0));
	camera->setData();
	
	setUpObject();
	//Qtree = new QuadTree(stataicObj, vec2(0, 0), 4, 200);
	Qtree = new QuadTree(stataicObj, vec2(0, 0), 4, 100);
	//(pi,3.14) = 180 degrees
	//Qtree->setUpCamProp(3.14/4, 50);
	Qtree->setUpCamProp(0.4, 2000);
	
	
 	bill = new BillBoard(gfx, vec3(0.f, 0.f, 9.f), rm->getFire(), rm->getDef()[1], 6);
	billManager = new BillBoardManager(gfx, rm->getFire(), 10, vec3(0,0,0),vec3(5,5,5));
	billManager->setAnimation(6, 1, 0.16);
	////DCube cannot use standard obj:s without fucking others shaders
	DCube = new DynamicCube(rm->get_Models("roundsol.obj"), gfx, vec3(5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	//DCube = new DynamicCube(rm->get_Models("DCube.obj"), gfx, vec3(5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f));
	/////LIGHT////////
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, light[i]->getPos(), light[i]->getRotation(), vec3(1.f, 1.0f, 1.0f)));
	}
	//UI
	//light
	for (int i = 0; i < nrOfLight; i++) {
		UIManager.takeLight(light[i]);
	}
	//camera
	UIManager.takeObject(obj[1]);
	
	
	gfx->takeLight((SpotLight**)light, nrOfLight);
	
	currentTime = 0;
	lightNr = 0;
}

Game::~Game() 
{
	//part of game
 	TC::GetInst().empty();
	delete gfx;
	delete rm;

	//logic and other
	delete defRend;
	delete mus;
	delete camera;
	if (shadowMap != nullptr) {
		delete shadowMap;
	}

	//objects
	for (int i = 0; i < LightVisualizers.size(); i++) {
		delete light[i];
		delete LightVisualizers[i];
	}
	delete[] light;
	for (int i = 0; i < obj.size(); i++) {
		delete obj[i];
	}
	for (int i = 0; i < stataicObj.size(); i++) {
		delete stataicObj[i];
	}
	delete Qtree;
	delete DCube;
	delete bill; 
	delete billManager;
}


void Game::run()
{
	static bool once = true;
	while (msg.message != WM_QUIT && once)
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
	//keyboard
	
	//obj[6]->addRot(vec3(0, 1.f * dt.dt(), 0));
	camera->updateCamera((float)dt.dt());
	if (getkey('N')) {
		DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-obj[1]->getPos().x, -obj[1]->getPos().y, -obj[1]->getPos().z, 1.0f
		);
		XRotation(viewMatrix, obj[1]->getRot().x);
		YRotation(viewMatrix, obj[1]->getRot().y);
		gfx->getVcb()->view.element = viewMatrix;
	}
	
	obj[0]->changePos(camera->getPos());
	obj[0]->changeRot(vec3(camera->getRot().z, camera->getRot().x, -camera->getRot().y) + vec3(0, 1.57, 0));
	bill->update((float)dt.dt());
	billManager->update(dt.dt(), gfx);
	mus->UpdateMouse();
	for (int i = 0; i < LightVisualizers.size(); i++) {
		LightVisualizers[i]->changePos(light[i]->getPos());
		LightVisualizers[i]->changeRot(vec3(0 , light[i]->getRotation().x, -light[i]->getRotation().y) + vec3(0,1.57,0));
	}
	
	gfx->Update((float)dt.dt(), camera->getPos());

#pragma region camera_settings
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
#pragma endregion camera_settings
}

void Game::DrawToBuffer()
{	
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->draw(gfx);
	}
    camera->calcFUL();
	if (getkey('Y')) {
		std::cout << "stop" << std::endl;
	}
	Qtree->draw(gfx, camera);
	Qtree->clearAlrDraw();

	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (int i = 0; i < LightVisualizers.size(); i++) {
		LightVisualizers[i]->drawDefTest(gfx->get_IC());
	}
}

//TODO : DO I NEED THIS?
void Game::DrawToBufferDebug()
{
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[0]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->drawDebug(gfx);
	}
}

void Game::DrawDynamicCube()
{
	gfx->Projection(2);
	vec3 camLP = camera->getPos();
	vec3 camRT = camera->getRot();
	vec3 DCRot = DCube->getRot();
	camera->setPosition(vec3(DCube->getPos()));
	camera->setRotation(vec3(DCube->getRot()));
	DCube->ClearRenderTarget(gfx);

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
		DCube->setViewPort(gfx);
		
		//DEFFERED RENDERING
		DCube->firstPass();//needed
		this->DrawToBufferDebug();
		DCube->secondPass(shadowMap->GetshadowResV(), DCube->getUAVs()[i], 16, 40, DCube->getCSShader());
		//FORWARD RENDERING
		DCube->setRenderTarget(gfx, i);
		this->ForwardDrawCube();
		
	}

	//draw the cube
	gfx->RsetViewPort();
	gfx->Projection(0);
	camera->setPosition(camLP);
	camera->setRotation(camRT);
	camera->updateCamera();
	if (getkey('N')) {
		DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-obj[1]->getPos().x, -obj[1]->getPos().y, -obj[1]->getPos().z, 1.0f
		);
		XRotation(viewMatrix, obj[1]->getRot().x);
		YRotation(viewMatrix, obj[1]->getRot().y);
		gfx->getVcb()->view.element = viewMatrix;
	}
	updateShaders(true, false);
	//PEM
	
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
	billManager->draw(gfx);
}

void Game::ForwardDrawCube()
{
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IC()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IC()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IC()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	gfx->get_IC()->HSSetShader(nullptr, nullptr, 0);
	gfx->get_IC()->DSSetShader(nullptr, nullptr, 0);
	bill->draw(gfx);
	billManager->draw(gfx);
}

void Game::DrawAllShadowObject()
{
	gfx->get_IC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IC()->IASetInputLayout(gfx->getInputL()[1]);
	gfx->get_IC()->GSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {

		obj[i]->draw(gfx, true);
	}
	camera->calcFUL();
	Qtree->draw(gfx, camera, true);
	Qtree->clearAlrDraw();
}

void Game::updateShaders(bool vs, bool ps)
{

	billManager->updateShader(gfx, camera->getPos());
	if (vs)
	{
		DCube->updateVertexShader(gfx);
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updateVertexShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
		for (int i = 0; i < stataicObj.size(); i++) {
			stataicObj[i]->updateVertexShader(gfx);
		}
	}
	if (ps) {
		DCube->updatePixelShader(gfx);
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < stataicObj.size(); i++) {
			stataicObj[i]->updatePixelShader(gfx);
		}
	}
}

void Game::setUpObject()
{
	////////OBJECTS///////////
	//cameras
	obj.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f)));//main
	obj.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, vec3(0.f, 100.f, 0.f), vec3(0.f, -1.58f, 0.f), vec3(2.f, 2.0f, 2.0f)));//second

	//OBJECTS
	obj.push_back(new GameObject(rm->get_Stol(), gfx, vec3(10.f, 5.f, 10.f), vec3(-1.56f, 1.56f, 3.2f), vec3(1.f, 1.f, 1.f)));
	obj.push_back(new GameObject(rm->get_Ball(), gfx, vec3(-5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	obj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(0.f, 0.f, -50.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	//walls
	obj.push_back(new GameObject(rm->get_IDK(), gfx, vec3(0.f, 5.f, 20.f),  vec3(-1.6f, -1.6f, 3.2f), vec3(20.f, 20.f, 20.f)));
	obj.push_back(new GameObject(rm->get_IDK(), gfx, vec3(20.f, 5.f, 0.f),  vec3(-1.6f, 0.f, 3.2f),   vec3(20.f, 20.f, 20.f)));
	obj.push_back(new GameObject(rm->get_IDK(), gfx, vec3(0.f, 5.f, -20.f), vec3(-1.6f, 1.6f, 3.2f),  vec3(20.f, 20.f, 20.f)));
	obj.push_back(new GameObject(rm->get_IDK(), gfx, vec3(-20.f, 5.f, 0.f), vec3(-1.6f, 3.f, 3.2f),   vec3(20.f, 20.f, 20.f)));
	
	//static
	stataicObj.push_back(new GameObject(rm->get_Models("stormtrooper.obj"),        gfx, vec3(25.f, 0.f, 0.f), vec3(0.f, 0.f, -1.57f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("stormtrooper.obj"),        gfx, vec3(0.f, 0.f, 25.f), vec3(0.f, 0.f, 1.57f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("stormtrooper.obj"),        gfx, vec3(-25.f, 0.f, 0.f), vec3(-1.57f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("stormtrooper.obj"),        gfx, vec3(0.f, 0.f, -25.f), vec3(1.57f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("Sting-Sword-lowpoly.obj"), gfx, vec3(25.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(0.3f, 0.3f, 0.3f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"),     gfx, vec3(25.f, 0.f, 5.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-37.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(37.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-37.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(37.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(12.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(12.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-12.5f, 0.f, 37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-12.5f, 0.f, -37.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(37.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-37.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(37.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-37.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(12.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(-12.5f, 0.f, 12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(12.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	stataicObj.push_back(new GameObject(rm->get_Models("indoor_plant_02.obj"), gfx, vec3(12.5f, 0.f, -12.5f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));
	int nrOfPlantsXY = 16;
	float sizeOfTree = 100 * 2;
	for (int x = 0; x < nrOfPlantsXY; x++) {
		for (int y = 0; y < nrOfPlantsXY; y++) {
			stataicObj.push_back(new GameObject(rm->get_IDK(),
				gfx, vec3(((sizeOfTree/ nrOfPlantsXY) * x) - sizeOfTree/2, 0.f, ((sizeOfTree / nrOfPlantsXY) * y) - sizeOfTree / 2),
				vec3(0.f, 0.f, 1.58f), vec3(5.f, 5.f, 5.f)));
		}
	}
	
	float gw = 10;
	float gn = 25;
	for (int x = 0; x < gn; x++) {
		for (int y = 0; y < gn; y++) {
			stataicObj.push_back(new GameObject(rm->get_IDK(), gfx, vec3(x*(gw*2) - ((gn)*gw), -4, y*(gw * 2) - ((gn)*gw)), vec3(0, 0, 1.57f), vec3(gw, gw, gw)));
		}
	}


	//obj[2]->setTesselation(false, gfx);
}
