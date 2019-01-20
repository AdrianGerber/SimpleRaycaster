#include "Renderer.h"

Renderer::Renderer(unsigned int xSize, unsigned int ySize) : raycaster(xSize)
{
	init(xSize, ySize);
}

Renderer::~Renderer()
{

}

void Renderer::setRenderMode(RenderMode newMode)
{
	currentRenderMode = newMode;
}

void Renderer::renderFrame(Camera& camera, Map& loadedMap)
{
	if (bIsActive) {
		/*Clear framebuffer*/
		renderWindow.clear(sf::Color::Black);

		RayIntersectionBuffer& buffer = raycaster.castRays(camera, loadedMap);

		switch (currentRenderMode) {
		case RenderMode::Line: lineRenderer.render(renderWindow, buffer, camera, loadedMap);
			break;
		case RenderMode::Map: mapRenderer.render(renderWindow, buffer, camera, loadedMap);
			break;
		case RenderMode::Polygon: polygonRenderer.render(renderWindow, buffer, camera, loadedMap);
			break;
		} 

		/*Push framebuffer to screen*/
		renderWindow.display();
	}
}

sf::RenderWindow & Renderer::getRenderWindow()
{
	return renderWindow;
}

void Renderer::resize(Camera & camera)
{
	deinit();
	init(camera.screenSize.x, camera.screenSize.y);
}

void Renderer::activateTextures(bool mode)
{
	lineRenderer.activateTextures(mode);
}

void Renderer::cycleRenderMode()
{
	if (currentRenderMode == RenderMode::Line) {
		setRenderMode(RenderMode::Map);
	}
	else if (currentRenderMode == RenderMode::Map) {
		setRenderMode(RenderMode::Polygon);
	}
	else {
		setRenderMode(RenderMode::Line);
	}
}

void Renderer::loadConfig(ConfigFile & config, Camera& camera)
{
	
	auto engineMode{ config.getAttribute("ENGINE_RENDERMODE") };
	auto engineTextures{ config.getFloatAttribute("ENGINE_ENABLETEXTURES") };

	if (engineTextures) {
		activateTextures(static_cast<bool>(*engineTextures));
	}

	if (engineMode) {
		if (*engineMode == "POLYGON") {
			setRenderMode(Renderer::RenderMode::Polygon);
		}
		else if (*engineMode == "LINE") {
			setRenderMode(Renderer::RenderMode::Line);
		}
		else if (*engineMode == "MAP") {
			setRenderMode(Renderer::RenderMode::Map);
		}
		else {
			std::cout << "Invalid Rendermode\n";
		}
	}
	resize(camera);
}

void Renderer::init(unsigned int xSize, unsigned int ySize)
{
	if (!bIsActive) {
		renderWindow.create({ xSize, ySize, 32 }, "3D-Renderer", sf::Style::Default);
		raycaster = Raycaster(xSize);
		bIsActive = true;
	}
}

void Renderer::deinit()
{
	if (bIsActive) {
		renderWindow.close();
		bIsActive = false;
	}
}
