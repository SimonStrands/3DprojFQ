#include "Mouse.h"

mouse::mouse()
{
	mouseSense = 1;
	pitch = 0;
	yaw = 0;
}

void mouse::changeSense(float newSence)
{
	this->mouseSense = newSence;
}

void mouse::OnRawDelta(int deltaX, int deltaY)
{
	pitch = deltaX;
	yaw = deltaY;
}

vec2 mouse::getDeltaPos()
{
	return vec2(pitch, yaw);
}

float mouse::getSense()
{
	return mouseSense;
}
