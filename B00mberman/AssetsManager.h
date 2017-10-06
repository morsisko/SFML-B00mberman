#pragma once
#include <SFML\Graphics.hpp>

class AssetsManager
{
private:
	sf::Texture tileTexture;
	sf::Font font;
public:
	AssetsManager();
	sf::Font& getFont();
	sf::Texture& getTileTexture();
	~AssetsManager();
};

