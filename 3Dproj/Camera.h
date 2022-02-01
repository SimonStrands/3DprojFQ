#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "rotation.h"
#include "Keyboard.h"
#include "Vec.h"
#include "Mouse.h"

class Camera {
public:
	Camera(Graphics *&gfx, Mouse *mus = nullptr, vec3 pos = vec3(0,0,10));
	virtual ~Camera();
	void updateCamera(float dt);
	vec3 getPos();
	void setPosition(vec3 newpos);
private:
	Mouse *mus;
	Vcb *Vcbd;
	LCBGS* Lcbd;
	void rotaiton(DirectX::XMMATRIX &matrix);
	void movement();
	void handleEvent(float dt);
	float speed;
	float mouseSensitivity;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float xCamRot;
	float yCamRot;
	DirectX::XMFLOAT3 translation;
	float movementspeed = 20;
	void Translate(float dt);
};