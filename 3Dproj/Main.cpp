#include <iostream>
#include "App.h"
#include "debug.h"
#include "Random.h"
//git


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//enable console
#ifdef _DEBUG
	RedirectIOToConsole();
#endif

	App app(hInstance, hPrevInstance, lpCmdLine, nCmdShow);	
	app.set_initial_gamestate(GameStatesEnum::TO_GAME);
	app.run();
	
	return 0;
}