#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "object.h"
#include <vector>
#include "Light.h"

class ImguiManager {
public:
	ImguiManager();
	~ImguiManager();
	void takeObject(object* obj);
	void takeLight(Light* light);
	void updateRender();
private:
	std::vector<object*> obj;
	std::vector<Light*> light;
};