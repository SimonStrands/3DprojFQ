#include "App.h"

App::App(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	settingsSingleTon::GetInst().readSettings();
	if (CoInitialize((LPVOID)NULL) != S_OK) {
		exit(1);
	}
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	mouse = gfx->getWindowClass().getMouse();
	keyboard = gfx->getWindowClass().getKeyboard();
	quit = false;

	defRend = new DeferredRendering(gfx);
	rm = new ResourceManager(gfx);

	camera = new Camera(gfx, mouse, keyboard, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	camera->setData();
	gfx->takeIM(&this->IMGUIManager);

	//Create a buffer for the light const buffer(hlsli)
	CreateConstBuffer(gfx, gfx->getConstBuffers(0), sizeof(*gfx->getLightconstbufferforCS()), gfx->getLightconstbufferforCS());
	CreateConstBuffer(gfx, gfx->getConstBuffers(1), sizeof(*gfx->getCamPosconstbuffer()), gfx->getCamPosconstbuffer());

}

App::~App()
{
	//part of game
	TC::GetInst().empty();
	delete gfx;
	delete rm;

	//logic and other
	delete defRend;
	delete gamestate;
	delete camera;
}


void App::run()
{
	GameStatesEnum gameState;
	gameState = GameStatesEnum::NO_CHANGE;

	while (msg.message != WM_QUIT && gfx->getWindowClass().ProcessMessages() && !quit)
	{
		gfx->UpdateFPSCounter(dt.getRealTime());
		//if (dt.dt() > 0.016f) {
		//	dt.setDeltaTime(0.016f);
		//}
		gfx->Update(camera->getPos());
		/*DEBUG MOUSE*/
#if defined _DEBUG
		if (getkey('M')) {
			mouse->activateMouse(true);
			gfx->getWindowClass().HideCoursor();
		}
		else if (getkey('N')) {
			mouse->activateMouse(false);
			gfx->getWindowClass().ShowCoursor();
		}
#endif


		//handle events
		gamestate->handleEvents();

		//Shadow
		gfx->clearScreen();
		gfx->setTransparant(false);
		//for shadow
		//måste uppdatera detta så inte hela object uppdateras när bara skugga ska
		vec3 camLP = camera->getPos();
		vec3 camLR = camera->getRot();
		gamestate->renderShadow();
		//set cam position so its the real cam
		camera->setPosition(camLP);
		camera->setRotation(camLR);
		gfx->setProjection(0);//last can be dir light
		gfx->RsetViewPort();


		gameState = gamestate->update((float)dt.dt());



		gamestate->render();
		this->mouse->set_captureEvent(true);
		handleGamestateChanges(gameState);
		mouse->clear();

		dt.restartClock();
	}
	printf("quit");
}

void App::set_initial_gamestate(GameStatesEnum gameStateType)
{
	if (gameStateType == GameStatesEnum::TO_GAME) {
		mouse->activateMouse(true);
		gfx->getWindowClass().HideCoursor();
		gamestate = new Game(gfx, rm, &IMGUIManager, mouse, keyboard, camera);
	}
	//example of other
	//else if (gameStateType == GameStatesEnum::TO_MENU) {
	//	mouse->activateMouse(true);
	//	gfx->getWindosClass().ShowCoursor();
	//	gamestate = new Menu(gfx, rm, &IMGUIManager, mouse, keyboard, camera);
	//}

}

void App::handleGamestateChanges(GameStatesEnum gameState)
{
	switch (gameState) {
	case GameStatesEnum::QUIT:
		quit = true;
		break;
	case GameStatesEnum::TO_GAME:
		this->mouse->set_captureEvent(false);
		mouse->activateMouse(true);
		gfx->getWindowClass().HideCoursor();
		//delete current gamestate
		delete gamestate;
		//set gamestate to Game
		gamestate = new Game(gfx, rm, &IMGUIManager, mouse, keyboard, camera);
		mouse->clearEventBuffer();

		break;

	//example of other
	//case GameStatesEnum::TO_MENU:
	//	mouse->activateMouse(true);
	//	gfx->getWindosClass().ShowCoursor();
	//	//delete current gamestate
	//	delete gamestate;
	//	//set gamestate to Menu
	//	gamestate = new Menu(gfx, rm, &IMGUIManager, mouse, keyboard, camera);
	//	break;
	}

}