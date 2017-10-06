#include "AssetsManager.h"



AssetsManager::AssetsManager()
{
	tileTexture.loadFromFile("assets/tile.png");
	font.loadFromFile("assets/ariblk.ttf");
}

sf::Font & AssetsManager::getFont()
{
	return font;
}

sf::Texture & AssetsManager::getTileTexture()
{
	return tileTexture;
}


AssetsManager::~AssetsManager()
{
}
