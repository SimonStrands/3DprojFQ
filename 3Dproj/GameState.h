#pragma once
#pragma once

#include "ResourceManager.h"
#include "Camera.h"
#include "Light.h"
#include "imguiManager.h"
#include "SettingsFlags.h"

enum class GameStatesEnum { NO_CHANGE, QUIT, TO_GAME };

class GameState {
public:
	GameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~GameState() = 0;
	//very important that they are done in order
	virtual void handleEvents() = 0;//this first
	virtual void renderShadow() = 0;//then this
	virtual GameStatesEnum update(float dt) = 0;//then this
	virtual void render() = 0;		//then this 
//protected:
public:
	Camera* camera;
	Graphics* gfx;
	ResourceManager* rm;
	ImguiManager* IMGUI;
	Mouse* mouse;
	Keyboard* keyboard;
};