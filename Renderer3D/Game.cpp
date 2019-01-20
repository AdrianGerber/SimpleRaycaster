#include "Game.h"



Game::Game()
{	
	player.camera.height = 0;
	player.camera.position = player.camera.position;
	player.camera.viewDistance = 30.0f;
	player.camera.screenSize = sf::Vector2u(400, 200);
	player.camera.fieldOfView = PI / 2.0f;
	player.camera.radius = 0.05f;

	renderer.resize(player.camera);

	measurementClock.restart();
}


Game::~Game()
{
}

bool Game::gameIsRunning()
{
	return renderer.isRunning();
}

void Game::update()
{
	sf::Event event;
	/*Handle SFML events*/
	while (renderer.getRenderWindow().pollEvent(event)) {
		/*Close window*/
		if (event.type == sf::Event::Closed) {
			renderer.deinit();
			return;
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::E) {
				renderer.cycleRenderMode();
			}
		}
	}

	float ellapsedTime{ measurementClock.restart().asSeconds() };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.move(Player::MoveDirection::RIGHTSIDE, ellapsedTime, currentMap);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.move(Player::MoveDirection::LEFTSIDE, ellapsedTime, currentMap);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.move(Player::MoveDirection::FORWARDS, ellapsedTime, currentMap);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.move(Player::MoveDirection::BACKWARDS, ellapsedTime, currentMap);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		std::cout << "Player: x: " << player.camera.position.x << " y: " << player.camera.position.y << " angle: " << player.camera.position.x << std::endl;
	}

	if (renderer.getRenderWindow().hasFocus() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		sf::Vector2i mouseCoordinates = sf::Mouse::getPosition(renderer.getRenderWindow());
		sf::Vector2i mouseOrigin = sf::Vector2i(player.camera.screenSize.x / 2, player.camera.screenSize.y / 2);
		sf::Vector2i mouseDelta = mouseCoordinates - mouseOrigin;
		sf::Mouse::setPosition(mouseOrigin, renderer.getRenderWindow());
		player.mouseInput(mouseDelta);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		renderer.getRenderWindow().setMouseCursorVisible(true);
	}
	else {
		renderer.getRenderWindow().setMouseCursorVisible(false);
	}
}

void Game::render()
{
	renderer.renderFrame(player.camera, currentMap);
}

void Game::loadConfigFiles()
{
	ConfigFile gameConfig;
	gameConfig.loadFile("game.conf");

	player.loadConfig(gameConfig);
	renderer.loadConfig(gameConfig, player.camera);

	auto mapdef{ gameConfig.getAttribute("MAPDEF") };
	auto mapname{ gameConfig.getAttribute("MAP") };

	if (mapdef && mapname) {
		loadMap(*mapdef, *mapname);
	}
	else {
		loadMap("default.mapdef", "default.map");
	}
}

void Game::loadMap(std::string mapdef, std::string mapname)
{
	std::cout << "Loading Map...\n";
	if (currentMap.loadFromFile(mapdef, mapname)) {
		player.camera.viewDistance = currentMap.getMaxViewDistance();
		player.camera.position = currentMap.getPlayerSpawnPosition();
		std::cout << "Map loaded\n";
	}
	else {
		std::cout << "Failed to load map (->Default Map)\n";
		player.camera.viewDistance = currentMap.getMaxViewDistance();
		player.camera.position = currentMap.getPlayerSpawnPosition();
	}
	
	
}
