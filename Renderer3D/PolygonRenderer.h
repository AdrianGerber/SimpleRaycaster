#pragma once
#include "RayIntersectionBuffer.h"
#include "MapTile.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>

class PolygonRenderer
{
public:
	PolygonRenderer();
	~PolygonRenderer();

	void render(sf::RenderWindow& renderWindow, RayIntersectionBuffer& rays, Camera& camera, Map& currentMap);

private:
	
	sf::ConvexShape polygonShape;
};

