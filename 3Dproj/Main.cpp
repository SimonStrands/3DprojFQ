#include <iostream>
#include "Game.h"
#include "debug.h"
//git
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//enable console
	RedirectIOToConsole();
	Game game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	game.run();
	
	
	return 0;
}