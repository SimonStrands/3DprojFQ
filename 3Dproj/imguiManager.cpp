#include "imguiManager.h"
#include <string>

ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}

void ImguiManager::takeObject(object* obj)
{
	this->obj.push_back(obj);
}

void ImguiManager::takeLight(Light* light)
{
	this->light.push_back(light);
}

void ImguiManager::updateRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	for (int i = 0; i < obj.size(); i++) {
		std::string name = "obj " + std::to_string(i);
		if (ImGui::Begin(name.c_str())) {
			ImGui::SliderFloat("Xpos", &obj[i]->getxPos(), 40.0f, -40.0f);
			ImGui::SliderFloat("Ypos", &obj[i]->getyPos(), 40.0f, -40.0f);
			ImGui::SliderFloat("Zpos", &obj[i]->getzPos(), 40.0f, -40.0f);
			ImGui::SliderFloat("XRot", &obj[i]->getxRot(), 20.0f, -20.0f);
		}
		ImGui::End();
	}
	for (int i = 0; i < light.size(); i++) {
		std::string name = "light " + std::to_string(i);
		if (ImGui::Begin(name.c_str())) {
			ImGui::SliderFloat("Xpos", &light[i]->getPos().x, 40.0f, -40.0f);
			ImGui::SliderFloat("Ypos", &light[i]->getPos().y, 40.0f, -40.0f);
			ImGui::SliderFloat("Zpos", &light[i]->getPos().z, 40.0f, -40.0f);
		}
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

