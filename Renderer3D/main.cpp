#include "Renderer.h"
#include <chrono>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ConfigFile.h"
#include "Game.h"

int main() {
	
	sf::Clock clk;
	float frameDuration = 0;
	float frameRate = 0;
	float elapsedTime = 0;
	unsigned int frameMeasurements = 0;

	Game game;
	game.loadConfigFiles();

	while (game.gameIsRunning()) {
		clk.restart();

		game.update();
		game.render();


		frameDuration = clk.getElapsedTime().asSeconds();
		elapsedTime += frameDuration;
		frameRate += (1.0f / frameDuration);
		frameMeasurements++;

		if (elapsedTime >= 5.0f) {
			elapsedTime = 0;

			frameRate /= static_cast<float>(frameMeasurements);

			std::cout << "FPS: " << frameRate << "\n";

			frameRate = 0;
			frameMeasurements = 0;
		}
	}

	std::cout << "Game Closed!";
	char c;
	std::cin >> c;
}