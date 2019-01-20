#pragma once
#include "MapTile.h"
#include "Map.h"
#include <memory>
#include <SFML/Graphics.hpp>

struct RayIntersection
{
public:
	bool intersectionFound;
	MapTile::WallDirection hitDirection;
	float distanceTraveled;
	float distanceToImageplane;
	float hitPositionOnWall;
	sf::Vector2f hitCoordinates;
	MapTileID tile;

	RayIntersection();
	~RayIntersection();
};

