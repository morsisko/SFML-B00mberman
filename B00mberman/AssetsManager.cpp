#include "AssetsManager.h"



AssetsManager::AssetsManager()
{
	tileTexture.loadFromFile("assets/tile.png");
}

sf::Texture & AssetsManager::getTileTexture()
{
	return tileTexture;
}


AssetsManager::~AssetsManager()
{
}
