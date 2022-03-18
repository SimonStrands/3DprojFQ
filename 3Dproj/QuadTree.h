#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "Camera.h"

struct QTCamData {
	vec3 CamPos;
	vec3 forwardVector;
	vec3 LeftNorm;
	vec3 RightNorm;
	vec3 UpNorm;
	vec3 DownNorm;
};

class QuadTree {
public:
	//create quad tree with a object list //only take in x and z, y is irrelevant.
	QuadTree(std::vector<GameObject*> &objectList, vec2 position, int depth, float size, bool fi = true);
	void setUpCamProp(float angle, float distanceFarPlane);
	~QuadTree();
	
	//have nodes
	//in private

	//have object list in last nodes
	//in private

	//draw
	void Sdraw(Graphics*& gfx, Camera* cam, bool sm = false);
	void draw(Graphics*& gfx, Camera* cam, bool sm = false);
	void clearAlrDraw();
	vec2 getPosition();
private:
	//objects
	int tisid;
	QuadTree *nodes[4];//have 4 section
	std::vector<GameObject*> ObjectList;
	//functions
	vec3 rotateX(float angle, vec3 vec);
	vec3 rotateY(float angle, vec3 vec);
	bool isInsideQuad(QuadTree node, vec3 camPos);
	bool pointInFront(vec3 point, vec3 cam);
	void sendQTCamData(QTCamData* qtCD);
	//data
	int depth;
	float size;
	vec3 position;
	bool first;
	//cam data
	float angle;
	float farPlane;
	QTCamData *qtCD;
};