#pragma once
#include "Vec.h"

class Bone {
public:
	//start Bone
	Bone();
	Bone(Bone* Parent, vec3 startPos, vec3 endPos);
	void giveChild(Bone *child);
private:
	Bone *child;
};