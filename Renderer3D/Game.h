#pragma once
#include "Player.h"
#include "Renderer.h"
#include "Map.h"
#include <SFML/Graphics.hpp>
#include "ConfigFile.h"

class Game
{
public:
	Game();
	~Game();

	bool gameIsRunning();
	void update();
	void render();

	void loadConfigFiles();
	void loadMap(std::string mapdef, std::string mapname);

private:
	bool bGameIsRunning;
	sf::Clock measurementClock;



	Player player;
	Renderer renderer;
	Map currentMap;
};

