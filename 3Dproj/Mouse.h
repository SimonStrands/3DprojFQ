#pragma once
#include "Vec.h"

class mouse {
public:
	mouse();
	void changeSense(float newSence);
	void OnRawDelta(int deltaX , int deltaY);
	vec2 getDeltaPos();
	float getSense();
private:
	float pitch, yaw;
	float mouseSense;
};