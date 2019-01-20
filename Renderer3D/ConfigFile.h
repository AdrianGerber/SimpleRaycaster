#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <optional>
#include "MapTile.h"

class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	void reset();
	void loadFile(const std::string& fileName);
	bool isLoaded();
	bool containsAttribute(const std::string& attribute);
	const std::optional<std::string> getAttribute(const std::string& attribute);

	const std::optional<sf::Vector3f> getVector3fAttribute(const std::string& attribute);
	const std::optional<sf::Vector2f> getVector2fAttribute(const std::string& attribute);
	const std::optional<float> getFloatAttribute(const std::string& attribute);

private:

	void replaceValueSeparators(std::string& str);

	std::string NameValueSeparators = " =";
	char optionalValueSeparator = ',';
	std::string name;
	bool attributesLoaded = false;
	std::map<std::string, std::string> content;
};

