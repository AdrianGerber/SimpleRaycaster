#pragma once
#include <SFML/Graphics.hpp>
#include "defs.h"

struct Camera
{
public:
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Vector2u screenSize;
	float fieldOfView;
	float height;
	float viewDistance;
	float radius;
	float calculateViewingOffset();
	float calculateDistanceFade(float distanceToCamera);
};

