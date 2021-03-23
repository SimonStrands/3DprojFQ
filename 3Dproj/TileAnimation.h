#pragma once
#include "Vec.h"

class TileAnimation {
public:
	//noah/w = nr of animation height/width,  
	//
	TileAnimation(int noah = 1, int noaw = 1, float timebeetween = 0.16, float width = 1.f, float height = 1.f);
	void update(float dt);
	vec4 uv();
private:
	float cpx, cpy;
	int noah, noaw;
	float timebeetween;
	float width, height;
	float timeToNextFrame;
};