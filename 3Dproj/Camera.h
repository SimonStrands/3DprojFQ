#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "rotation.h"
#include "Keyboard.h"
#include "Vec.h"
#include "Mouse.h"

class Camera {
public:
	Camera(Graphics *&gfx, Mouse *mus = nullptr);
	virtual ~Camera();
	void updateCamera(float dt);
	vec3 getPos();
private:
	Mouse *mus;
	Vcb *Vcbd;
	Pcb *Pcbd;
	void rotaiton(DirectX::XMMATRIX &matrix);
	void movement();
	void handleEvent(float dt);
	Graphics* gfx;
	float speed;
	float mouseSensitivity;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float xCamRot;
	float yCamRot;
	DirectX::XMFLOAT3 translation;
	float movementspeed = 10;
	void Translate(float dt);
};