#include "Map.h"

Map::Map(size_t xSize, size_t ySize)
{
	mapXSize = xSize;
	mapYSize = ySize;

	maxViewDistance = sqrt(static_cast<float>(mapXSize*mapXSize) + static_cast<float>(mapYSize*mapYSize));

	mapTiles.clear();
	mapTiles.reserve(xSize*ySize);
	spawnPosition.x = 5.0f;
	spawnPosition.y = 5.0f;
	maxViewDistance = 10.0f;

	createDefaultTiles(mapTiles);

	/*Initialize map with empty tiles*/
	for (size_t index{ 0 }; index < xSize*ySize; index++) {
		mapGrid.push_back(defaultSpace);
	}

	/*Create walls around the outside*/
	for (size_t xPosition{ 0 }; xPosition < xSize; xPosition++) {
		setTile(xPosition, 0, defaultWall);
		setTile(xPosition, ySize - 1, defaultWall);

	}
	for (size_t yPosition{ 0 }; yPosition < ySize; yPosition++) {
		setTile(0, yPosition, defaultWall);
		setTile(xSize - 1, yPosition, defaultWall);
	}
	update();
}

Map::~Map()
{
}

MapTileID Map::getTileID(size_t xPos, size_t yPos)
{
	/*Map the coordinates to positions inside the vector*/
	return mapGrid[yPos * mapXSize + xPos];
}

MapTile & Map::getMapTile(MapTileID id)
{
	return mapTiles[id];
}

void Map::setTile(size_t xPos, size_t yPos, MapTileID tile)
{
	mapGrid[yPos * mapXSize + xPos] = tile;
}

sf::Vector2u Map::getSize()
{
	return sf::Vector2u(mapXSize, mapYSize);
}

bool Map::fastWallCheck(size_t xPos, size_t yPos)
{
	return mapWalls[yPos * mapXSize + xPos];
}

void Map::update()
{
	updateWallMap();
	//maxViewDistance = sqrt(static_cast<float>(mapXSize*mapXSize) + static_cast<float>(mapYSize*mapYSize));
}

float Map::getMaxViewDistance()
{
	return maxViewDistance;
}

sf::Vector2f Map::getPlayerSpawnPosition()
{
	return spawnPosition;
}

MapTileID Map::getDefaultWall()
{
	return defaultWall;
}

MapTileID Map::getDefaultSpace()
{
	return defaultSpace;
}

bool Map::loadFromFile(std::string mapdefName, std::string mapName)
{
	ConfigFile mapdef;
	mapdef.loadFile(mapdefName);
	auto mapTileNames{ mapdef.getAttribute("MAPTILES") };

	auto mapViewDistance{ mapdef.getFloatAttribute("PLAYER_VIEWDISTANCE") };
	float newViewDistance{ 10.0f };
	if (mapViewDistance) {
		newViewDistance = *mapViewDistance;
	}

	std::vector<MapTile> newMapTiles;
	std::vector<MapTileID> newMapGrid;
	createDefaultTiles(newMapTiles);
	std::map<char, MapTileID> mapTileNamesToIDs;
	mapTileNamesToIDs.emplace(' ', defaultSpace);
	mapTileNamesToIDs.emplace('X', defaultWall);

	if (mapTileNames) for (auto currentTileName : *mapTileNames) {
		auto mapTileData{ mapdef.getAttribute(std::string(1, currentTileName)) };
		MapTile newMapTile;
		if (mapTileData && newMapTile.createFromString(*mapTileData)) {
			mapTileNamesToIDs.emplace(currentTileName, newMapTiles.size());
			newMapTiles.push_back(newMapTile);
		}
	}

	std::ifstream mapFile;// { mapName };
	mapFile.open( mapName );
	if (!mapFile.is_open()) return false;
	
	sf::Vector2u newMapSize{ 0, 0 };
	std::string currentLine;
	sf::Vector2f newPlayerSpawnposition{ 5.0f, 5.0f };

	size_t xPosition{ 0 };
	size_t yPosition{ 0 };
	while(std::getline(mapFile, currentLine)){
		if (currentLine.length() == 0) continue;
		if (newMapSize.x == 0) newMapSize.x = currentLine.length();

		if (currentLine.length() != newMapSize.x) {
			mapFile.close();
			return false;
		}

		for (auto c : currentLine) {
			if (c == 'P') {
				newPlayerSpawnposition.x = xPosition + 0.5f;
				newPlayerSpawnposition.y = yPosition + 0.5f;
				newMapGrid.push_back(defaultSpace);
			}
			else if (mapTileNamesToIDs.find(c) != mapTileNamesToIDs.end()) {
				newMapGrid.push_back(mapTileNamesToIDs[c]);
			}
			else {
				newMapGrid.push_back(defaultWall);
			}
			xPosition++;
		}
		xPosition = 0;
		yPosition++;
		newMapSize.y++;
	}

	mapFile.close();

	mapGrid = newMapGrid;
	mapTiles = newMapTiles;
	mapXSize = newMapSize.x;
	mapYSize = newMapSize.y;
	spawnPosition = newPlayerSpawnposition;
	maxViewDistance = newViewDistance;

	update();

	return true;
}

void Map::updateWallMap()
{
	mapWalls.clear();
	mapWalls.reserve(mapXSize*mapYSize);
	
	for (size_t index{ 0 }; index < mapXSize*mapYSize; index++) {
		mapWalls.push_back(mapTiles[(mapGrid[index])].shouldRenderAsWall());
	}
}

MapTileID Map::registerNewMapTile(MapTile tile)
{
	mapTiles.push_back(tile);
	return static_cast<MapTileID>(mapTiles.size() - 1);
}

MapTileID Map::registerNewMapTile(MapTile tile, std::vector<MapTile>& storage)
{
	storage.push_back(tile);
	return static_cast<MapTileID>(storage.size() - 1);
}

void Map::createDefaultTiles(std::vector<MapTile>& storage){
	/*Create tiles needed for default map*/
	defaultTexture = std::make_shared<sf::Texture>();
	
	defaultTexture->loadFromFile("square.jpg", sf::IntRect());

	MapTile emptyTile;
	emptyTile.setAsWall(false);
	emptyTile.setColor(sf::Color::Black);
	//emptyTile.setTexture(defaultTexture);

	MapTile wallTile;
	wallTile.setAsWall(true);
	wallTile.setColor(sf::Color::White);
	//wallTile.setTexture(defaultTexture);

	storage.clear();
	defaultSpace = registerNewMapTile(emptyTile, storage);
	defaultWall = registerNewMapTile(wallTile, storage);
}
