#pragma once
#include <vector>
#include <memory>
#include "MapTile.h"
#include "Map.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include "ConfigFile.h"

using MapTileID = size_t;

class Map
{
public:
	Map(size_t xSize = 10, size_t ySize = 10);
	~Map();

	MapTileID getTileID(size_t xPos, size_t yPos);
	MapTile& getMapTile(MapTileID id);
	void setTile(size_t xPos, size_t yPos, MapTileID);
	sf::Vector2u getSize();
	bool fastWallCheck(size_t xPos, size_t yPos);
	void update();
	float getMaxViewDistance();
	sf::Vector2f getPlayerSpawnPosition();
	MapTileID getDefaultWall();
	MapTileID getDefaultSpace();

	bool loadFromFile(std::string mapdefName, std::string mapName);

private:
	void updateWallMap();
	
	MapTileID registerNewMapTile(MapTile tile);
	MapTileID registerNewMapTile(MapTile tile, std::vector<MapTile>& storage);

	std::vector<MapTileID> mapGrid;
	/*Should use bool, but performance increases by 33% when using bytes*/
	std::vector<uint8_t> mapWalls;
	std::vector<MapTile> mapTiles;

	std::shared_ptr<sf::Texture> defaultTexture;

	void createDefaultTiles(std::vector<MapTile>& storage);
	MapTileID defaultWall;
	MapTileID defaultSpace;

	size_t mapXSize;
	size_t mapYSize;
	float maxViewDistance;

	sf::Vector2f spawnPosition;
};

