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
	sf::Texture& texture;

	std::array<std::array<TileType, MAP_WIDTH>, MAP_HEIGHT> logicArray;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getTileIndexFromTileType(TileType type);
	TileType intToTileType(int type);
public:
	Level(sf::Texture& texture);
	void load(std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& levelData);
	bool isValidPosition(int x, int y);
	bool isPointCollidable(float x, float y);
	bool isLogicPointCollidable(sf::Vector2i position);
	void setTileAsType(int x, int y, TileType tileType);
	sf::Vector2f getRealPositionFromLogicPosition(int x, int y);
	sf::Vector2i getLogicPositionFromRealPosition(float x, float y);
	~Level();
};

