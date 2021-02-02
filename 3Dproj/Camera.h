#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "rotation.h"
#include "Keyboard.h"
#include "Vec.h"
//git
class Camera {
public:
	Camera(Graphics *&gfx);
	virtual ~Camera();
	void updateCamera(DeltaTime dt);
	vec3 getPos();
private:
	Vcb *Vcbd;
	Pcb *Pcbd;
	void rotaiton(DirectX::XMMATRIX &matrix);
	void movement();
	void handleEvent(DeltaTime dt);
	Graphics* gfx;
	float speed;
	float mouseSensitivity;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float xCamRot;
	float yCamRot;
};