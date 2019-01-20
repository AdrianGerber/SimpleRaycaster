#include "MapRenderer.h"



MapRenderer::MapRenderer()
{
	cameraPositionOnScreen.x = 0.5f;
	cameraPositionOnScreen.y = 0.9f;
}


MapRenderer::~MapRenderer()
{
}

void MapRenderer::render(sf::RenderWindow& renderWindow, RayIntersectionBuffer& rays, Camera& camera, Map& currentMap)
{
	/*How big will one block be drawn on screen?*/
	/*The distance between the camera's position on screen and the top border shall contain all objects within the
	camera's maximal vieweing distance*/
	float mapScale{ (camera.screenSize.y) / camera.viewDistance };
	mapScale /= cameraPositionOnScreen.y;
	/*Calculate the camera's onscreen position (stays fixed during movement)*/
	sf::Vector2f origin{ camera.screenSize.x * cameraPositionOnScreen.x, camera.screenSize.y*cameraPositionOnScreen.y };

	/*Draw the camera*/
	float circleSize{ camera.radius * mapScale };
	circleShape.setOrigin(circleSize, circleSize);
	circleShape.setPosition(origin);
	circleShape.setFillColor(sf::Color::White);
	circleShape.setRadius(circleSize);
	renderWindow.draw(circleShape);


	/*Draw all the visible tiles*/
	/*How many blocks fit in the largest dimension of the screen*/
	float blocksOnScreen{ fmaxf(camera.screenSize.x,camera.screenSize.y) / mapScale };
	blocksOnScreen += 1.0f;	//Also draw blocks that are only partially inside the screen

	/*Only draw blocks inside a square with a sidelength of the screen's largest dimension (Inefficient, but better than no filtering at all)*/
	sf::Vector2i startOffset{ static_cast<int>(-blocksOnScreen), static_cast<int>(-blocksOnScreen) };
	sf::Vector2i endOffset{ static_cast<int>(blocksOnScreen), static_cast<int>(blocksOnScreen) };
	sf::Vector2i currentOffset{ 0, 0 };											//Offset camera to maptile
	sf::Vector2i cameraPosition{ static_cast<sf::Vector2i>(camera.position) };  //Position of the camera as integer
	sf::Vector2i currentBlockPosition{ 0, 0 };									//Resulting coordinates
	/*Go through every possible x-/y-offset combination*/
	for (currentOffset.x = startOffset.x; currentOffset.x < endOffset.x; currentOffset.x++) {
		for (currentOffset.y = startOffset.y; currentOffset.y < endOffset.y; currentOffset.y++) {
			/*Calculate the maptile's position*/
			currentBlockPosition = currentOffset + cameraPosition;
			/*Make sure that the position is valid*/
			if (currentBlockPosition.x >= 0 && currentBlockPosition.y >= 0) {
				/*Draw the corresponding block*/
				drawBlock(renderWindow, camera, currentMap, static_cast<sf::Vector2u>(currentBlockPosition), mapScale, origin);
			}
		}
	}

	/*Show the camera's field of view using lines (rectangles with width 1)*/
	rectangleShape.setFillColor(sf::Color::Red);
	rectangleShape.setRotation(180.0f - ((camera.fieldOfView / 2.0f) / (2 * PI) * 360.0f));
	rectangleShape.setPosition(origin);
	rectangleShape.setSize(sf::Vector2f(1.0f, rays.rayIntersections[0].distanceTraveled * mapScale));
	renderWindow.draw(rectangleShape);
	rectangleShape.setPosition(origin);
	rectangleShape.setRotation(180.0f + ((camera.fieldOfView / 2.0f) / (2 * PI) * 360.0f));
	rectangleShape.setSize(sf::Vector2f(1.0f, rays.rayIntersections[rays.rayIntersections.size() - 1].distanceTraveled * mapScale));
	renderWindow.draw(rectangleShape);

	/*Setup variables to draw all rays*/
	float currentAngle{ ((-PI / 2.0f) - (camera.fieldOfView / 2)) };							//angle of ray, initialized with rightmost ray
	float angleStep{ camera.fieldOfView / static_cast<float>(camera.screenSize.x) };			//angle increment between two sequential rays
	sf::Vector2f currentRayDirection{ 0.0f, 0.0f };												//Unit vector with the same direction as the ray
	sf::Vector2f hitPointSize{ fmaxf(mapScale / 10.0f, 1.0f), fmaxf(mapScale / 10.0f, 1.0f) };  //Size of the rectangle drawn at the ray's end
	rectangleShape.setOrigin(hitPointSize.x / 2.0f, hitPointSize.y / 2.0f);						//Place the rectangle using center coordinates

	/*Go through every ray that was cast*/
	for (auto& x : rays.rayIntersections) {
		/*Don't draw anything if no wall was found within the camera's viewing distance*/
		if (!x.intersectionFound) {
			/*The current angle still needs to be incremented...*/
			currentAngle += angleStep;
			continue;
		}

		/*Calculate this ray's direction (Unit vector)*/
		currentRayDirection.x = cos(currentAngle);
		currentRayDirection.y = sin(currentAngle);
		/*Scale it to match the ray's traveled distance and the scale of the map*/
		currentRayDirection *= x.distanceTraveled;
		currentRayDirection *= mapScale;
		/*Place and draw the rectangle*/
		rectangleShape.setOrigin(0.0f, 0.0f);
		rectangleShape.setRotation(0.0f);
		rectangleShape.setPosition(currentRayDirection + origin);
		rectangleShape.setSize(hitPointSize);
		rectangleShape.setFillColor(sf::Color::Red);
		renderWindow.draw(rectangleShape);

		/*Increment angle to represent the next ray*/
		currentAngle += angleStep;
	}
}

void MapRenderer::drawBlock(sf::RenderWindow& renderWindow, Camera& camera, Map& currentMap, sf::Vector2u blockPosition, float mapScale, sf::Vector2f screenOrigin)
{
	/*Make sure the coordinates are valid for the current map*/
	if (blockPosition.x < currentMap.getSize().x && blockPosition.y < currentMap.getSize().y && currentMap.fastWallCheck(blockPosition.x, blockPosition.y)) {
		/*Vector between the camera's position and the block's position*/
		sf::Vector2f delta = { (static_cast<sf::Vector2f>(blockPosition) - camera.position) * mapScale };
		delta.x += mapScale; //add one block offset, so the vector ends at the upper left corner (map's first index is 0!)

		/*Transform the map coordinates into screen coordinates*/
		sf::Vector2f screenDelta{ cos(-camera.direction.x)*delta.x - sin(-camera.direction.x)*delta.y,
			sin(-camera.direction.x)*delta.x + cos(-camera.direction.x)*delta.y
		};

		/*Swap components, because the screen is 'upside down': (0,0) => top left*/
		float tmp{ screenDelta.x };
		screenDelta.x = screenDelta.y;
		screenDelta.y = -tmp;

		/*Draw the rectangle*/
		rectangleShape.setOrigin(0.0f, 0.0f);									//Set position using the top left corner
		rectangleShape.setSize(sf::Vector2f(mapScale, mapScale));				//Draw as square, 1 unit length
		rectangleShape.setPosition(screenOrigin + screenDelta);
		rectangleShape.setFillColor(sf::Color::White);
		rectangleShape.setRotation(-(camera.direction.x / (2 * PI)) * 360.0f);	//Rotate in the opposite direction of the camera's rotation
		renderWindow.draw(rectangleShape);
	}
}