#include "MapTile.h"

MapTile::MapTile(sf::Color initialColor)
{
	cXn = cXp = cYn = cYp = initialColor;
	isWall = false;
}

MapTile::~MapTile()
{
}

sf::Color MapTile::getColor(WallDirection face)
{
	switch (face) {
		case WallDirection::Xn: return(cXn);
		case WallDirection::Xp: return(cXp);
		case WallDirection::Yn: return(cYn);
		case WallDirection::Yp: return(cYp);
		default: return cXn;
	}
}

void MapTile::setColor(WallDirection face, sf::Color newColor)
{
	switch (face) {
	case WallDirection::Xn: cXn = newColor; break;
	case WallDirection::Xp: cXp = newColor; break;
	case WallDirection::Yn: cYn = newColor; break;
	case WallDirection::Yp: cYp = newColor; break;
	default: cXn = newColor; break;
	}
}

void MapTile::setColor(sf::Color newColor)
{
	cXn = cXp = cYn = cYp = newColor;
}


void MapTile::setTexture(std::shared_ptr<sf::Texture> newTexture)
{
	hasTexture = true;
	texture = newTexture;
}

std::shared_ptr<sf::Texture> MapTile::getTexture(void)
{
	return texture;
}

void MapTile::setAsWall(bool newState)
{
	isWall = newState;
}

bool MapTile::createFromString(std::string & input)
{
	std::replace(input.begin(), input.end(), ',', ' ');
	std::stringstream inputStream(input);
	return createFromStringStream(inputStream);
}

bool MapTile::createFromStringStream(std::stringstream & input)
{
	sf::Color colors[4];
	unsigned int r, g, b;

	for (size_t i{ 0 }; i < 4; i++) {		
		if (!(input >> r && input >> g && input >> b)) return false;

		colors[i].r = r;
		colors[i].g = g;
		colors[i].b = b;
	}

	cXn = colors[0];
	cXp = colors[1];
	cYn = colors[2];
	cYp = colors[3];

	std::string texturename;
	if (input >> texturename) {
		std::shared_ptr<sf::Texture> newTexture{ std::make_shared<sf::Texture>() };
		if (newTexture->loadFromFile(texturename, sf::IntRect())) {
			setTexture(newTexture);
		}
		else {
			std::cout << "Texture " << texturename << " not found\n";
		}
	}

	setAsWall(true);
	return true;
}

bool MapTile::shouldRenderTexture()
{
	return hasTexture;
}

bool MapTile::shouldRenderAsWall()
{
	return isWall;
}
