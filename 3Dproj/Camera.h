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
	void getViewFrustoms(vec3 frustoms[], float angle);
	void setRotation(vec3 newRot);
	void setPosition(vec3 newpos);
	void setData(float FOVRadians = 90, float viewRatio = 16/9, float nearDist = 0.001, float farDist = 2000);
private:
	Mouse *mus;
	Vcb *Vcbd;
	LCBGS* Lcbd;
	void rotaiton(DirectX::XMMATRIX &matrix);
	void rotaiton(DirectX::XMMATRIX& matrix, float xRot, float yRot);
	void movement();
	void handleEvent(float dt);

	vec3 FUL[3];//Forward, Up, Left
	DirectX::XMFLOAT3 translation;
	float movementspeed = 20;
	void Translate(float dt);

	//data
	float speed;
	float mouseSensitivity;
	float xCamPos;
	float yCamPos;
	float zCamPos;
	float xCamRot;
	float yCamRot;
	float zCamRot;

	//other data
	float ratio;
	float fov;
	float nearDist;
	float farDist;
};