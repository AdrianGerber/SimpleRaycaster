#include "LineRenderer.h"



LineRenderer::LineRenderer()
{
	drawTextures = true;
}


LineRenderer::~LineRenderer()
{
}

void LineRenderer::render(sf::RenderWindow & renderWindow, RayIntersectionBuffer & rays, Camera & camera, Map& currentMap)
{
	int startingPosition = camera.calculateViewingOffset();
	/*Go through every column of the screen*/
	for (unsigned int xPosition{ 0 }; xPosition < camera.screenSize.x; xPosition++) {
		if (!rays.rayIntersections[xPosition].intersectionFound) {
			continue;
		}
		/*Calculate start and endpoint of the wall*/
		float distanceToImageplane = rays.rayIntersections[xPosition].distanceToImageplane;
		int wallHeight = static_cast<int>(static_cast<float>(camera.screenSize.y) / distanceToImageplane);
		int wallYStart = startingPosition - (wallHeight / 2);	
		int wallYEnd = startingPosition + (wallHeight / 2);
		/*Store raw start and endpoint*/
		int wallCalcYStart = wallYStart;										
		int wallCalcYEnd = wallYEnd;
		/*Start and endpoint guaranteed to be within the area of the screen*/
		if (wallYEnd >= camera.screenSize.y) wallYEnd = camera.screenSize.y - 1;
		if (wallYStart < 0) wallYStart = 0;
		/*Get the color of the maptile*/
		sf::Color tileColor = currentMap.getMapTile(rays.rayIntersections[xPosition].tile).getColor(rays.rayIntersections[xPosition].hitDirection);
		/*Fade color with distance*/
		float colorScale = camera.calculateDistanceFade(rays.rayIntersections[xPosition].distanceTraveled);
		/*Slightly change color depending on direction the wall is facing*/
		sf::Vector3f directionFactor(1.0f, 1.0f, 1.0f);
		if (rays.rayIntersections[xPosition].hitDirection == MapTile::WallDirection::Xn || rays.rayIntersections[xPosition].hitDirection == MapTile::WallDirection::Xp) {
			directionFactor.x = 0.8f;
			directionFactor.y = 0.8f;
			directionFactor.z = 0.8f;
		}
		/*Apply these values to the maptile's color*/
		sf::Color drawColor(tileColor.r * colorScale * directionFactor.x, tileColor.g * colorScale * directionFactor.y, tileColor.b * colorScale * directionFactor.z, 255);

		/*Should walls be rendered with textures?*/
		if (drawTextures && currentMap.getMapTile(rays.rayIntersections[xPosition].tile).shouldRenderTexture()) {
			/*Get the texture of the maptile that was hit by the ray*/
			std::shared_ptr<sf::Texture> texture{
				currentMap.getMapTile(rays.rayIntersections[xPosition].tile).getTexture()
			};
			/*Apply the texture to the line*/
			lineShape.setTexture(&(*texture), 0);

			/*Calculate how much of the texture is obstructed*/
			int totalObstructed = 0;
			if (wallCalcYEnd >= camera.screenSize.y) totalObstructed += wallCalcYEnd - camera.screenSize.y;	//Endpoint above maximum
			if (wallCalcYStart < 0) totalObstructed += -wallCalcYStart;										//Startpoint below minimum 

			/*Vertical start and endpoint of the texture*/
			int textureYStart = 0;
			int textureYHeight = texture->getSize().y;
			/*If part of the texture is obstructed, calculate the start and height of the visible portion in texture coordinates*/
			if (totalObstructed > 0) {
				textureYHeight = texture->getSize().y - (texture->getSize().y * static_cast<float>(totalObstructed) / static_cast<float>(wallHeight));
				textureYStart = static_cast<int>(texture->getSize().x * ((static_cast<float>((wallCalcYStart < 0) ? (abs(wallCalcYStart)) : (0)) / static_cast<float>(wallCalcYEnd - wallCalcYStart))));
			}
			/*Select the calculated area of the texture*/
			lineShape.setTextureRect(sf::IntRect(rays.rayIntersections[xPosition].hitPositionOnWall * (texture->getSize().x), textureYStart, 1, textureYHeight));
		}
		else {
			lineShape.setTexture(NULL, false);
		}
		
		/*Draw the column*/
		lineShape.setFillColor(drawColor);
		lineShape.setPosition(static_cast<float>(xPosition), static_cast<float>(wallYStart));
		lineShape.setSize(sf::Vector2f(1.0f, static_cast<float>(wallYEnd - wallYStart)));
		renderWindow.draw(lineShape);
	}
}

void LineRenderer::activateTextures(bool state)
{
	drawTextures = state;
}
