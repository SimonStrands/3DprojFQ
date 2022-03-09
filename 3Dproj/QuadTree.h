#pragma once
#include "GameObject.h"
class QuadTree {
public:
	//create quad tree with a object list //only take in x and z, y is irrelevant.
	QuadTree(GameObject** objectList, int nrOf, vec2 position, int depth, float size);
	~QuadTree();
	
	//have nodes
	//in private

	//have object list in last nodes
	//in private

	//update that update
	void update();
private:
	QuadTree *nodes[4];//have 4 section
	std::vector<GameObject*> ObjectList;
	bool last;
	int depth;
	float size;
	/*
	s = size;
	| |s|
	-----
	| | |
	-----
	| | |
	*/
};