#pragma once

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "MapTile.h"
#include "RayIntersection.h"
#include "RayIntersectionBuffer.h"
#include "Camera.h"
#include <cmath>

class Raycaster
{
public:
	Raycaster(unsigned int xSize);
	void setResolution(unsigned int xSize);
	RayIntersectionBuffer& castRays(Camera& camera, Map& currentMap);
private:
	static float calculateDistanceToPoint(float deltaX, float deltaY);
	static float calculateDistanceToPlane(float deltaX, float deltaY, float angle);

	RayIntersectionBuffer rayIntersectionBuffer;
};
