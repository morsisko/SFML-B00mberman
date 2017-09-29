#pragma once
#include "SFML\Graphics.hpp"
#include <array>

enum TileType
{
	GRASS = 0,
	BORDER_BLOCK,
	SPACE_BLOCK,
	DIRT
};

class Level : public sf::Drawable
{
private:
	static const int MAP_HEIGHT = 13;
	static const int MAP_WIDTH = 15;
	sf::VertexArray vertex;
	sf::Texture texture;

	static const int TILE_SIZE = 16;
	static const int DISPLAY_TILE_SIZE = 48;


	std::array<std::array<TileType, MAP_WIDTH>, MAP_HEIGHT> logicArray;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getTileIndexFromTileType(TileType type);
	TileType intToTileType(int type);
public:
	Level();
	void load(std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& levelData);
	bool isValidPosition(int x, int y);
	bool isNonCollidable(int x, int y);
	void setTileAsType(int x, int y, TileType tileType);
	~Level();
};

