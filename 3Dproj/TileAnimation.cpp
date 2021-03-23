#include "TileAnimation.h"

TileAnimation::TileAnimation(int noah, int noaw, float timebeetween, float width, float height)
{
	this->noah = noah;
	this->noaw = noaw;
	this->timebeetween = timebeetween;
	this->width = width;
	this->height = height;
	cpx = cpy = 0;
	timeToNextFrame = 0;
}

void TileAnimation::update(float dt)
{
	//width / noah
	timeToNextFrame += dt;
	if (timeToNextFrame > timebeetween) {
		timeToNextFrame = 0;
		cpx++;
		if (cpx >= noaw) {
			cpx = 0;
		}
	}
	
}

vec4 TileAnimation::uv()
{
	return vec4(cpx, cpy, cpx + (width / noaw), cpy + (height / noah));
}
