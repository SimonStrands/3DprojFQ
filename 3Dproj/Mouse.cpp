#include "Mouse.h"

Mouse::Mouse()
{
	mouseSense = 1;
	pitch = 0;
	yaw = 0;
}

void Mouse::changeSense(float newSence)
{
	this->mouseSense = newSence;
}

void Mouse::OnRawDelta(int deltaX, int deltaY)
{
	pitch = deltaX;
	yaw = deltaY;
}

vec2 Mouse::getDeltaPos()
{
	return vec2(pitch, yaw);
}

float Mouse::getSense()
{
	return mouseSense;
}
