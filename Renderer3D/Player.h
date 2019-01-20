#pragma once

#include <SFML/Graphics.hpp>
#include "defs.h"
#include <cmath>
#include "Map.h"
#include "Camera.h"
#include "ConfigFile.h"

class Player
{
public:
	Camera camera;

	enum class MoveDirection{FORWARDS, BACKWARDS, LEFTSIDE, RIGHTSIDE};
	enum class TurnDirection{CCW, CW};

	Player();
	~Player();

	void setMovementSpeed(float newSpeed);
	void setTurningSpeed(float newSpeed);

	void move(MoveDirection direction, float elapsedTime, Map& currentMap);
	void mouseInput(sf::Vector2i move);

	void loadConfig(ConfigFile& config);

private:
	float movementSpeed = 3.0f;
	float turningSpeed = 4.0f;
	sf::Vector2f mouseSensitivity;
};

