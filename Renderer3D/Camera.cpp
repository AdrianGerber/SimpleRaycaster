#include "Camera.h"

float Camera::calculateViewingOffset()
{
	return screenSize.y *abs(direction.y - (PI / 4)) / (PI / 2);
}

float Camera::calculateDistanceFade(float distanceToCamera)
{
	if (distanceToCamera > viewDistance || distanceToCamera < 0.0f) return 0.0f;

	float colorFactor = 1.0f - (distanceToCamera / viewDistance);

	return colorFactor;
}
