#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "rotation.h"
#include "Keyboard.h"
#include "Vec.h"
#include "Mouse.h"

class Camera {
public:
	Camera(Graphics *&gfx, Mouse *mus = nullptr, vec3 pos = vec3(0,0,10), vec3 rot = vec3(0,0,1));
	virtual ~Camera();
	void updateCamera();
	void updateCamera(float dt);
	vec3 getPos();
	vec3 getRot();
	void calcFUL();
	vec3 getForwardVec();
	vec3 getUpVector();
	vec3 getLeftVector();
	void setRotation(vec3 newRot);
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
	float zCamRot;
	vec3 FUL[3];//Forward, Up, Left
	DirectX::XMFLOAT3 translation;
	float movementspeed = 20;
	void Translate(float dt);
};