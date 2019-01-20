#include "PolygonRenderer.h"



PolygonRenderer::PolygonRenderer()
{
	polygonShape.setPointCount(4);
}


PolygonRenderer::~PolygonRenderer()
{
}

void PolygonRenderer::render(sf::RenderWindow& renderWindow, RayIntersectionBuffer& rays, Camera& camera, Map& currentMap)
{
	int startingPosition = camera.calculateViewingOffset();					//Vectical offset to simulate looking up/down/jumping/etc.
	int polygonStartX = 0;													//Startposition for the polygon that will be drawn next
	/*IDs*/
	MapTileID startID = currentMap.getDefaultSpace();						//MapTile at the beginning of the polygon
	MapTileID lastID = startID;												//ID of the previous column's wall
	MapTileID currentID = startID;											//ID of the wall at current column
	/*Directions*/
	MapTile::WallDirection startDirection = MapTile::WallDirection::Xn;		//Direction of the wall at the beginning of the polygon
	MapTile::WallDirection lastDirection = MapTile::WallDirection::Xn;		//Direction of the previous column's wall
	MapTile::WallDirection currentDirection = MapTile::WallDirection::Xn;	//Direction of the wall at the current column
	/*Distances*/
	float startDistance = 0;												//Distance camera to wall at the starting column of the polygon
	float lastDistance = 0;													//Distance to the wall at the last column
	float currentDistance = 0;												//Distance at the current column
	/*Hitpositions*/
	sf::Vector2f lastHitPosition;											//Coordinates where the ray of the last column hit the wall
	sf::Vector2f currentHitPosition;										//Coordinates where the current column's ray hit a wall
	bool distanceJump = false;												//Can the last and current column be drawn using the same polygon, or is the distance too big?


	/*Go through every column of the screen and draw the walls using a sf::ConvexShape for each coninous section*/
	for (size_t xPosition{ 0 }; xPosition < camera.screenSize.x; xPosition++) {
		/*Load the current columns information*/
		currentID = rays.rayIntersections[xPosition].tile;
		currentDirection = rays.rayIntersections[xPosition].hitDirection;
		currentDistance = rays.rayIntersections[xPosition].distanceToImageplane;
		currentHitPosition = rays.rayIntersections[xPosition].hitCoordinates;

		//lastTileIsAdjacent = fabsf(currentHitPosition.x - lastHitPosition.x) + fabsf(currentHitPosition.y - lastHitPosition.y) < 0.5f;//(static_cast<int>(currentHitPosition.x) == static_cast<int>(lastHitPosition.x)) || ((currentHitPosition.y) == static_cast<int>(lastHitPosition.y));
		
		/*Check if the current and last column show the same 'wall' (the distance between the two hitpositions should not be more than 1 mapTile)*/
		/*If this check is left out, walls will sometimes be drawn using the same polygon if the other attributes are identical*/
		/*This will force a new polygon to be drawn, if the positions of the last and current maptile aren't adjacent*/
		sf::Vector2i delta(static_cast<int>(currentHitPosition.x) - static_cast<int>(lastHitPosition.x), static_cast<int>(currentHitPosition.y) - static_cast<int>(lastHitPosition.y));
		distanceJump = !(abs(delta.x) + abs(delta.y) <= 1);

		/*Is this column's wall different from the last one's*/
		if (currentID != startID || currentDirection != startDirection || xPosition == camera.screenSize.x - 1 || distanceJump) {
			/*Calculate the previous and next index, limited to valid values*/
			size_t lastPosition = 0;
			size_t nextPosition = xPosition + 1;
			if (xPosition > 0) lastPosition = xPosition - 1;
			if (nextPosition >= camera.screenSize.x) nextPosition = camera.screenSize.x - 1;
			/*Load attributes of the next column's wall*/
			MapTileID nextID = rays.rayIntersections[nextPosition].tile;
			MapTile::WallDirection nextDirection = rays.rayIntersections[nextPosition].hitDirection;
			float nextDistance = rays.rayIntersections[nextPosition].distanceToImageplane;
			sf::Vector2f nextHitPosition = rays.rayIntersections[nextPosition].hitCoordinates;
			/*Calculte if there is a jump in distance between the next and the current wall*/
			delta.x = static_cast<int>(currentHitPosition.x) - static_cast<int>(lastHitPosition.x);
			delta.y = static_cast<int>(currentHitPosition.y) - static_cast<int>(lastHitPosition.y);
			distanceJump = !(abs(delta.x) + abs(delta.y) <= 1);
			/*Ignore the current column, if it is 1 pixel wide and the next and previous walls are the same*/
			if (lastID == nextID && lastDirection == nextDirection && xPosition != camera.screenSize.x - 1 && !distanceJump) {
				/*Skip this column's values*/
				currentID = lastID;
				currentHitPosition = lastHitPosition;
				currentDirection = lastDirection;
				currentDistance = lastDistance;
			}
			/*The polygon needs to be drawn*/
			else {
				
				/*Calculate coordinates and dimensions for the starting position of the polygon*/
				if (startDistance == 0) startDistance += 0.001f;	//Don't allow division by 0
				int wallHeight = static_cast<int>(static_cast<float>(camera.screenSize.y) / startDistance);
				int wallYStart = startingPosition - (wallHeight / 2);
				int wallYEnd = startingPosition - (wallHeight / 2);
				/*Limit the values to the inside of the screen*/
				if (wallYEnd >= camera.screenSize.y) wallYEnd = camera.screenSize.y - 1;
				if (wallYStart < 0) wallYStart = 0;

				/*Set the coordinates of the points at the start of the polygon*/
				polygonShape.setPoint(0, sf::Vector2f(polygonStartX, startingPosition - (wallHeight / 2)));
				polygonShape.setPoint(1, sf::Vector2f(polygonStartX, startingPosition + (wallHeight / 2)));

				/*Repeat the calculation using the last column's data*/
				if (lastDistance == 0) lastDistance += 0.001f;
				wallHeight = static_cast<unsigned int>(static_cast<float>(camera.screenSize.y) / lastDistance);
				wallYStart = startingPosition - (wallHeight / 2);
				wallYEnd = startingPosition - (wallHeight / 2);
				if (wallYEnd >= camera.screenSize.y) wallYEnd = camera.screenSize.y - 1;
				if (wallYStart < 0) wallYStart = 0;

				/*Set the last two points of the polygon*/
				polygonShape.setPoint(2, sf::Vector2f(xPosition, startingPosition + (wallHeight / 2)));
				polygonShape.setPoint(3, sf::Vector2f(xPosition, startingPosition - (wallHeight / 2)));

				/*Slightly adjust the polygon's fillcolor, depending on the direction of the wall*/
				sf::Vector3f colorScale(1.0f, 1.0f, 1.0f);
				if (lastDirection == MapTile::WallDirection::Xn || lastDirection == MapTile::WallDirection::Xp) colorScale = sf::Vector3f(0.9f, 0.9f, 0.9f);
				sf::Color fillColor = currentMap.getMapTile(lastID).getColor(lastDirection); //Load the unprocessed color from the map
				fillColor.r *= colorScale.x;												 //Scale the brightness
				fillColor.g *= colorScale.y;
				fillColor.b *= colorScale.z;
				polygonShape.setFillColor(fillColor);										 //Use the new color

				/*This polygon is complete, now draw it*/
				renderWindow.draw(polygonShape);

				/*Store the details of this row, which marks the beginning of the next polygon*/
				startDistance = currentDistance;
				startID = currentID;
				startDirection = currentDirection;
				polygonStartX = xPosition;
			}
		}

		/*Always keep the last iteration's values in memory*/
		lastID = currentID;
		lastDirection = currentDirection;
		lastDistance = currentDistance;
		lastHitPosition = currentHitPosition;
	}
}
