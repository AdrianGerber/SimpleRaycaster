#include "ConfigFile.h"



ConfigFile::ConfigFile()
{
}


ConfigFile::~ConfigFile()
{
}

void ConfigFile::reset()
{
	attributesLoaded = false;
	content.clear();
}

void ConfigFile::loadFile(const std::string & fileName)
{
	attributesLoaded = false;
	std::cout << "Loading config file: " << fileName << "...\n";
	std::ifstream configFile;
	configFile.open(fileName);

	if (!configFile.is_open()) {
		std::cout << "Can't open file:" << fileName << std::endl;
		return;
	}

	std::string currentLine;
	std::string attribute;
	std::string value;

	while (std::getline(configFile, currentLine)) {
		if (currentLine.length() && currentLine[0] == '#') continue;
		
		size_t attributeEnd{ currentLine.find_first_of(NameValueSeparators) };
		if (attributeEnd == std::string::npos || attributeEnd == 0) continue;
		size_t valueStart{ currentLine.find_first_not_of(NameValueSeparators, attributeEnd) };
		if (valueStart == std::string::npos) continue;

		attribute = currentLine.substr(0, attributeEnd);
		value = currentLine.substr(valueStart);

		//std::cout << fileName << "::" << attribute << "=" << value << "\n";
		content.emplace(attribute, value);
	}

	std::cout << "Config Loaded!\n";
	attributesLoaded = true;
	configFile.close();
}

bool ConfigFile::isLoaded()
{
	return attributesLoaded;
}

bool ConfigFile::containsAttribute(const std::string & attribute)
{
	return attributesLoaded && (content.find(attribute) != content.end());
}

const std::optional<std::string> ConfigFile::getAttribute(const std::string & attribute)
{
	std::optional<std::string> returnString;

	if (containsAttribute(attribute)) {
		std::cout << attribute << " = " << content[attribute] << "\n";
		returnString.emplace(content[attribute]);
	}


	return returnString;
}

const std::optional<sf::Vector3f> ConfigFile::getVector3fAttribute(const std::string & attribute)
{
	std::optional<sf::Vector3f> returnValue;
	sf::Vector3f tmpVect;

	if (!containsAttribute(attribute)) return returnValue;

	std::string attributeString{ content[attribute] };
	replaceValueSeparators(attributeString);
	std::stringstream attributeStream{ attributeString };

	if ((attributeStream >> tmpVect.x)
		&& (attributeStream >> tmpVect.y)
		&& (attributeStream >> tmpVect.z)) {
		returnValue.emplace(tmpVect);
	}

	std::cout << attribute << " = " << content[attribute] << "\n";
	return returnValue;
}

const std::optional<sf::Vector2f> ConfigFile::getVector2fAttribute(const std::string & attribute)
{
	std::optional<sf::Vector2f> returnValue;
	sf::Vector2f tmpVect;

	if (!containsAttribute(attribute)) return returnValue;

	std::string attributeString{ content[attribute] };
	replaceValueSeparators(attributeString);
	std::stringstream attributeStream{ attributeString };

	if ((attributeStream >> tmpVect.x)
		&& (attributeStream >> tmpVect.y)) {
		returnValue.emplace(tmpVect);
	}

	std::cout << attribute << " = " << content[attribute] << "\n";
	return returnValue;
}

const std::optional<float> ConfigFile::getFloatAttribute(const std::string & attribute)
{
	std::optional<float> returnValue;
	float tmpFloat;

	if (!containsAttribute(attribute)) return returnValue;

	std::string attributeString{ content[attribute] };
	replaceValueSeparators(attributeString);
	std::stringstream attributeStream{ attributeString };

	if (attributeStream >> tmpFloat) {
		returnValue.emplace(tmpFloat);
	}

	std::cout << attribute << " = " << content[attribute] << "\n";
	return returnValue;
}

void ConfigFile::replaceValueSeparators(std::string & str)
{
	std::replace(str.begin(), str.end(), optionalValueSeparator, ' ');
}
