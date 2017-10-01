#pragma once
#include <SFML\Graphics.hpp>

class AssetsManager
{
private:
	sf::Texture tileTexture;
public:
	AssetsManager();
	sf::Texture& getTileTexture();
	~AssetsManager();
};

