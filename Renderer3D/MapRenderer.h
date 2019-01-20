#pragma once
#include "RayIntersectionBuffer.h"
#include "MapTile.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>

class MapRenderer
{
public:
	MapRenderer();
	~MapRenderer();

	void render(sf::RenderWindow& renderWindow, RayIntersectionBuffer& rays, Camera& camera, Map& currentMap);

private:

	void drawBlock(sf::RenderWindow& renderWindow, Camera& camera, Map& currentMap, sf::Vector2u blockPosition, float mapScale, sf::Vector2f screenOrigin);

	sf::RectangleShape rectangleShape;
	sf::CircleShape circleShape;
	sf::Vector2f cameraPositionOnScreen;
};

