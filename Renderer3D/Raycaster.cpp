#include "Raycaster.h"

Raycaster::Raycaster(unsigned int xSize = 400) : rayIntersectionBuffer(xSize)
{
}

void Raycaster::setResolution(unsigned int xSize)
{
	rayIntersectionBuffer = RayIntersectionBuffer(xSize);
}

RayIntersectionBuffer& Raycaster::castRays(Camera& camera, Map& currentMap)
{
	float centerAngle = camera.direction.x;
	unsigned int rayCount = rayIntersectionBuffer.rayIntersections.size();

	float rayAngle = (centerAngle - (camera.fieldOfView / 2));
	float rayAngleStep = (camera.fieldOfView / static_cast<float>(rayCount));

	sf::Vector2u mapSize = currentMap.getSize();
	sf::Vector2f direction;
	sf::Vector2f nearestGridPosition;
	sf::Vector2f directionOffset;
	sf::Vector2f integerPositionX;
	sf::Vector2f integerPositionY;
	sf::Vector2f xUnitStep;
	sf::Vector2f yUnitStep;
	sf::Vector2f xClosestWallTile;
	sf::Vector2f yClosestWallTile;

	/*Calculate the distance to the nearest wall at the corresponding viewing angle for each row of pixels*/
	for (unsigned int rayNumber{ 0 }; rayNumber < rayCount; rayNumber++) {
		/*Unit vector -> the ray's direction*/
		direction.x = cos(rayAngle);
		direction.y = sin(rayAngle);

		/*Closest integer coordinates in the calculated direction*/
		nearestGridPosition.x = (direction.x > 0) ? ceil(camera.position.x) : floor(camera.position.x);
		nearestGridPosition.y = (direction.y > 0) ? ceil(camera.position.y) : floor(camera.position.y);

		/*Values for conversion between map coordinates and tile index (depending on direction of ray)*/
		if (direction.x >= 0) directionOffset.x = 0;
		else directionOffset.x = -1.0f;
		if (direction.y >= 0) directionOffset.y = 0;
		else directionOffset.y = -1.0f;

		/*Starting coordinates of the ray, aligned with either the x- or y-grid*/
		integerPositionX.x = 0;
		integerPositionX.y = 0;
		integerPositionY.x = 0;
		integerPositionY.y = 0;

		if (direction.x != 0) {
			/*Move startingposition along the ray, until x is a whole number*/
			integerPositionX = camera.position + direction * abs(nearestGridPosition.x - camera.position.x) / abs(direction.x);
			/*Small offset to compensate rounding error (prevents looking through walls when standing too close)*/
			integerPositionX.x += 0.0001f;
		}
		if (direction.y != 0) {
			/*Move startingposition along the ray, until x is a whole number*/
			integerPositionY = camera.position + direction * abs(nearestGridPosition.y - camera.position.y) / abs(direction.y);
			/*Small offset to compensate rounding error (prevents looking through walls when standing too close)*/
			integerPositionY.y += 0.0001f;
		}

		/*Vector between two x-gridlines, parallel to the ray*/
		xUnitStep.x = 1;
		xUnitStep.y = abs(tan(rayAngle));
		if (direction.x < 0) xUnitStep.x = -xUnitStep.x;
		if (direction.y < 0) xUnitStep.y = -xUnitStep.y;

		/*Vector between two y-gridlines, parallel to the ray*/
		yUnitStep.y = 1;
		yUnitStep.x = 0;
		if (rayAngle != 0.0f) {
			yUnitStep.x = abs(1 / tan(rayAngle));
		}
		if (direction.x < 0) yUnitStep.x = -yUnitStep.x;
		if (direction.y < 0) yUnitStep.y = -yUnitStep.y;

		float xPlaneDistance = 0;									//Distance from the image plane to this tile
		float xTraveledDistance = 0;								//Distance between the starting point and the tile
		float yPlaneDistance = 0;									//Distance from the image plane to this tile
		float yTraveledDistance = 0;								//Distance between the starting point and the tile
		bool xWallFound = false;									//Has a tile been found using x steps?
		bool yWallFound = false;									//Has a tile been found using y steps?

		unsigned int xRemainingSteps = (camera.viewDistance / sqrt((xUnitStep.x * xUnitStep.x) + (xUnitStep.y * xUnitStep.y))) + 1;
		unsigned int yRemainingSteps = (camera.viewDistance / sqrt((yUnitStep.x * yUnitStep.x) + (yUnitStep.y * yUnitStep.y))) + 1;

		/*Attempt to find the closest wall tile using x-steps*/
		while (integerPositionX.x + directionOffset.x < mapSize.x && integerPositionX.x + directionOffset.x >= 0 && integerPositionX.y >= 0 && integerPositionX.y < mapSize.y && xUnitStep.y != 0.0f && !xWallFound && xRemainingSteps) {
			/*Check if the tile should be rendered as a wall*/
			if (currentMap.fastWallCheck(static_cast<size_t>(integerPositionX.x + directionOffset.x), static_cast<size_t>(integerPositionX.y))) {
				/*Store tile information and calculate the distance*/
				xClosestWallTile = sf::Vector2f(integerPositionX.x + directionOffset.x, integerPositionX.y);
				sf::Vector2f delta = integerPositionX - camera.position;
				xPlaneDistance = calculateDistanceToPlane(delta.x, delta.y, centerAngle);
				xTraveledDistance = calculateDistanceToPoint(delta.x, delta.y);
				xWallFound = true;
				break;
			}
			/*Move to the next tile*/
			integerPositionX += xUnitStep;
			xRemainingSteps--;
		}

		/*Attempt to find the closest wall tile using y-steps*/
		while (integerPositionY.y + directionOffset.y < mapSize.y && integerPositionY.y + directionOffset.y >= 0 && integerPositionY.x < mapSize.x && integerPositionY.x >= 0 && yUnitStep.x != 0.0f && !yWallFound && yRemainingSteps) {
			/*Check if the tile should be rendered as a wall*/
			if (currentMap.fastWallCheck(static_cast<size_t>(integerPositionY.x), static_cast<size_t>(integerPositionY.y + directionOffset.y))) {
				/*Store tile information and calculate the distance*/
				yClosestWallTile = sf::Vector2f(integerPositionY.x, integerPositionY.y + directionOffset.y);
				sf::Vector2f delta = integerPositionY - camera.position;
				yPlaneDistance = calculateDistanceToPlane(delta.x, delta.y, centerAngle);
				yTraveledDistance = calculateDistanceToPoint(delta.x, delta.y);
				yWallFound = true;
				break;
			}
			/*Move to the next tile*/
			integerPositionY += yUnitStep;
			yRemainingSteps--;
		}

		/*was any wall hit by the ray?*/
		if (xWallFound || yWallFound) {
			/*Was a wall found using x steps and is it closer than the wall found using y steps */
			if (xWallFound && (!yWallFound || (xPlaneDistance < yPlaneDistance))) {
				/*Store distance, map tile, position of hit and the direction the wall was hit from*/
				rayIntersectionBuffer.rayIntersections[rayNumber].intersectionFound = true;
				rayIntersectionBuffer.rayIntersections[rayNumber].distanceTraveled = xTraveledDistance;
				rayIntersectionBuffer.rayIntersections[rayNumber].distanceToImageplane = xPlaneDistance;
				rayIntersectionBuffer.rayIntersections[rayNumber].tile = currentMap.getTileID(static_cast<size_t>(xClosestWallTile.x), static_cast<size_t>(xClosestWallTile.y));
				rayIntersectionBuffer.rayIntersections[rayNumber].hitPositionOnWall = xClosestWallTile.y - floor(xClosestWallTile.y);
				rayIntersectionBuffer.rayIntersections[rayNumber].hitDirection =
					(directionOffset.x < 0) ?
					(MapTile::WallDirection::Xp) :
					(MapTile::WallDirection::Xn);
				rayIntersectionBuffer.rayIntersections[rayNumber].hitCoordinates = xClosestWallTile;
			}
			/*Was a wall found using y steps?*/
			else if (yWallFound) {
				/*Store distance, map tile, position of hit and the direction the wall was hit from*/
				rayIntersectionBuffer.rayIntersections[rayNumber].intersectionFound = true;
				rayIntersectionBuffer.rayIntersections[rayNumber].distanceTraveled = yTraveledDistance;
				rayIntersectionBuffer.rayIntersections[rayNumber].distanceToImageplane = yPlaneDistance;
				rayIntersectionBuffer.rayIntersections[rayNumber].tile = currentMap.getTileID(static_cast<size_t>(yClosestWallTile.x), static_cast<size_t>(yClosestWallTile.y));
				rayIntersectionBuffer.rayIntersections[rayNumber].hitPositionOnWall = yClosestWallTile.x - floor(yClosestWallTile.x);
				rayIntersectionBuffer.rayIntersections[rayNumber].hitDirection =
					(directionOffset.y < 0) ?
					(MapTile::WallDirection::Yp) :
					(MapTile::WallDirection::Yn);
				rayIntersectionBuffer.rayIntersections[rayNumber].hitCoordinates = yClosestWallTile;
			}
		}
		/*No wall could be found*/
		else {
			/*Place default tile at the highest distance found on the map*/
			rayIntersectionBuffer.rayIntersections[rayNumber].intersectionFound = false;
			rayIntersectionBuffer.rayIntersections[rayNumber].distanceTraveled = camera.viewDistance;
			rayIntersectionBuffer.rayIntersections[rayNumber].distanceToImageplane = camera.viewDistance;
			rayIntersectionBuffer.rayIntersections[rayNumber].hitPositionOnWall = 0.0f;
			rayIntersectionBuffer.rayIntersections[rayNumber].tile = currentMap.getDefaultWall();
			rayIntersectionBuffer.rayIntersections[rayNumber].hitDirection = MapTile::WallDirection::Xn;
			rayIntersectionBuffer.rayIntersections[rayNumber].hitCoordinates.x = 0.0f;
			rayIntersectionBuffer.rayIntersections[rayNumber].hitCoordinates.y = 0.0f;
		}
		
		rayAngle += rayAngleStep;
	}

	return rayIntersectionBuffer;
}

float Raycaster::calculateDistanceToPoint(float deltaX, float deltaY)
{
	return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}

float Raycaster::calculateDistanceToPlane(float deltaX, float deltaY, float angle)
{
	return(((deltaX)* cos(angle)) + (deltaY)* sin(angle));
}
