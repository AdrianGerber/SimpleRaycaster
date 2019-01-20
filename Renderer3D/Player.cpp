#include "Player.h"



Player::Player()
{
	movementSpeed = 3.0f;
	turningSpeed = 1.5f;
	mouseSensitivity = sf::Vector2f(0.01f, 0.01f);
	camera.direction = sf::Vector2f(0.0f, 0.0f);
	camera.fieldOfView = PI / 2.0f;
	camera.height = 0;
	camera.position = sf::Vector2f(3.0f, 3.0f);
	camera.radius = 0.3f;
	camera.screenSize = sf::Vector2u(400, 200);
	camera.viewDistance = 20;
}


Player::~Player()
{
}

void Player::setMovementSpeed(float newSpeed)
{
	movementSpeed = newSpeed;
}

void Player::setTurningSpeed(float newSpeed)
{
	turningSpeed = newSpeed;
}

void Player::move(MoveDirection direction, float ellapsedTime, Map& currentMap)
{
	sf::Vector2f movement(0.0f, 0.0f);
	if (direction == MoveDirection::BACKWARDS || direction == MoveDirection::FORWARDS) {
		movement.x = cos(camera.direction.x) * movementSpeed * ellapsedTime;
		movement.y = sin(camera.direction.x) * movementSpeed * ellapsedTime;
		movement = (direction == MoveDirection::FORWARDS) ? (movement) : (-movement);
	}

	else if (direction == MoveDirection::LEFTSIDE || direction == MoveDirection::RIGHTSIDE) {
		movement.x = cos(camera.direction.x + (PI / 2.0f)) * movementSpeed * ellapsedTime;
		movement.y = sin(camera.direction.x + (PI / 2.0f)) * movementSpeed * ellapsedTime;
		movement = (direction == MoveDirection::LEFTSIDE) ? (movement) : (-movement);
	}

	sf::Vector2f newPosition = camera.position + movement;
	if (currentMap.fastWallCheck(static_cast<size_t>(newPosition.x), static_cast<size_t>(newPosition.y))) {
		;
	}
	else {
		camera.position = newPosition;
	}
}
void Player::mouseInput(sf::Vector2i move)
{
	camera.direction.x += static_cast<float>(move.x) * mouseSensitivity.x;
	camera.direction.y += static_cast<float>(move.y) * mouseSensitivity.y;

	if (camera.direction.y > PI / 4.0f) camera.direction.y = PI / 4.0f;
	if (camera.direction.y < -( PI / 4.0f)) camera.direction.y = -(PI / 4.0f);
}

void Player::loadConfig(ConfigFile & config)
{
	auto engineResolution{ config.getVector2fAttribute("ENGINE_RESOLUTION") };
	if (engineResolution) {
		camera.screenSize = static_cast<sf::Vector2u>(*engineResolution);
	}

	auto newMouseSensitivity{ config.getVector2fAttribute("MOUSE_SENSITIVITY") };
	if (newMouseSensitivity) {
		mouseSensitivity = *newMouseSensitivity;
	}
}
