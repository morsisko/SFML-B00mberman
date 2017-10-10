#pragma once
#include "SFML\Graphics.hpp"
#include <array>
#include <deque>
#include "Bomb.h"

enum TileType
{
	GRASS = 0,
	BORDER_BLOCK,
	SPACE_BLOCK,
	DIRT
};

class Level : public sf::Drawable
{
public:
	static const int MAP_HEIGHT = 13;
	static const int MAP_WIDTH = 15;

private:
	sf::VertexArray vertex;
	sf::Texture& texture;

	std::array<std::array<TileType, MAP_WIDTH>, MAP_HEIGHT> logicArray;
	std::deque<Bomb> bombs;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getTileIndexFromTileType(TileType type);
	TileType intToTileType(int type);
	void load(std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& levelData);
public:
	Level(sf::Texture& texture, std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& levelData);
	bool isValidPosition(int x, int y);
	bool isPointCollidable(float x, float y);
	bool isLogicPointCollidable(sf::Vector2i position);
	void setTileAsType(int x, int y, TileType tileType);
	void putBomb(int id, sf::Vector2i position);
	void update(const sf::Time& deltaTime);
	sf::Vector2f getRealPositionFromLogicPosition(int x, int y);
	sf::Vector2i getLogicPositionFromRealPosition(float x, float y);
	~Level();
};

