#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

class MapTile
{
public:
	enum class WallDirection { Xn = 0, Xp = 1, Yn = 2, Yp = 3};

	MapTile(sf::Color initialColor = sf::Color::White);
	~MapTile();

	sf::Color getColor(WallDirection face);
	void setColor(WallDirection face, sf::Color newColor);
	void setColor(sf::Color newColor);

	void setTexture(std::shared_ptr<sf::Texture> newTexture);
	std::shared_ptr<sf::Texture> getTexture(void);

	bool shouldRenderTexture();
	bool shouldRenderAsWall();
	void setAsWall(bool newState);

	bool createFromString(std::string& input);
	bool createFromStringStream(std::stringstream& input);


private:
	bool hasTexture = false;
	bool isWall = true;
	sf::Color cXn, cXp, cYn, cYp;
	std::shared_ptr<sf::Texture> texture;
};

