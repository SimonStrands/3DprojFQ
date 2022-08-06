#include "GameState.h"

GameState::GameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam)
{
	this->gfx = gfx;
	this->rm = rm;
	this->IMGUI = imguimanager;
	this->camera = cam;
	this->keyboard = keyboard;
	this->mouse = mouse;
}

GameState::~GameState()
{
}

