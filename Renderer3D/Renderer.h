#pragma once

#include <cmath>
#include <iostream>
#include "Map.h"
#include "MapTile.h"
#include "Player.h"
#include "RayIntersectionBuffer.h"
#include "RayIntersection.h"
#include "Raycaster.h"
#include <cstdint>
#include "LineRenderer.h"
#include "MapRenderer.h"
#include "PolygonRenderer.h"
#include <SFML/OpenGL.hpp>
#include "ConfigFile.h"

class Renderer
{
public:
	enum class RenderMode {Line, Map, Polygon};

	Renderer(unsigned int xSize = 400, unsigned int ySize = 200);
	~Renderer();

	void setRenderMode(RenderMode newMode);

	void renderFrame(Camera& camera, Map& loadedMap);
	bool isRunning() { return bIsActive; }
	sf::RenderWindow& getRenderWindow();

	void init(unsigned int xSize, unsigned int ySize);
	void deinit();
	void resize(Camera& camera);

	void activateTextures(bool mode);


	void cycleRenderMode();

	void loadConfig(ConfigFile& config, Camera& camera);
private:
	bool bIsActive = false;

	sf::RenderWindow renderWindow;

	RenderMode currentRenderMode;
	Raycaster raycaster;
	LineRenderer lineRenderer;
	MapRenderer mapRenderer;
	PolygonRenderer polygonRenderer;
};

