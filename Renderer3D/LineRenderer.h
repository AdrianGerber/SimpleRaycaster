#pragma once


#include "RayIntersectionBuffer.h"
#include "Map.h"
#include "MapTile.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>

class LineRenderer
{
public:
	LineRenderer();
	~LineRenderer();

	void render(sf::RenderWindow& renderWindow, RayIntersectionBuffer& rays, Camera& camera, Map& currentMap);

	void activateTextures(bool state);

private:
	bool drawTextures;
	sf::RectangleShape lineShape;
};

