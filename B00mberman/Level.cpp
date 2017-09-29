#include "Level.h"



void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &texture;
	target.draw(vertex, states);
}

sf::Vector2i Level::getTileIndexFromTileType(TileType type)
{
	if (type == GRASS)
		return sf::Vector2i(1, 0);

	else if (type == BORDER_BLOCK)
		return sf::Vector2i(10, 5);

	else if (type == SPACE_BLOCK)
		return sf::Vector2i(10, 4);
	
	return sf::Vector2i(0, 0);
	
}

TileType Level::intToTileType(int type)
{
	return static_cast<TileType>(type);
}

Level::Level()
{
	texture.loadFromFile("assets/tile.png");

	vertex.setPrimitiveType(sf::Quads);
	vertex.resize(MAP_WIDTH * MAP_HEIGHT * 4);
}

void Level::load(std::array<std::array<int, 15>, 13>& levelData)
{
	for (int i = 0; i < MAP_WIDTH; ++i)
	{
		for (int j = 0; j < MAP_HEIGHT; ++j)
		{
			TileType tileType = intToTileType(levelData[j][i]);
			setTileAsType(i, j, tileType);
		}
	}
}

bool Level::isValidPosition(int x, int y)
{
	return x < MAP_WIDTH && x >= 0 && y < MAP_HEIGHT && y >= 0;
}

bool Level::isNonCollidable(int x, int y)
{
	return isValidPosition(x, y) && logicArray[y][x] == GRASS || logicArray[y][x] == DIRT;
}

void Level::setTileAsType(int x, int y, TileType tileType)
{
	if (!isValidPosition(x, y))
		return;

	logicArray[y][x] = tileType;
	sf::Vector2i fillTexture = getTileIndexFromTileType(tileType);
	sf::Vertex* quad = &vertex[(x + y * MAP_WIDTH) * 4];

	// define its 4 corners
	quad[0].position = sf::Vector2f(x * DISPLAY_TILE_SIZE, y * DISPLAY_TILE_SIZE);
	quad[1].position = sf::Vector2f((x + 1) * DISPLAY_TILE_SIZE, y * DISPLAY_TILE_SIZE);
	quad[2].position = sf::Vector2f((x + 1) * DISPLAY_TILE_SIZE, (y + 1) * DISPLAY_TILE_SIZE);
	quad[3].position = sf::Vector2f(x * DISPLAY_TILE_SIZE, (y + 1) * DISPLAY_TILE_SIZE);

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(fillTexture.x * TILE_SIZE, fillTexture.y * TILE_SIZE);
	quad[1].texCoords = sf::Vector2f((fillTexture.x + 1) * TILE_SIZE, fillTexture.y * TILE_SIZE);
	quad[2].texCoords = sf::Vector2f((fillTexture.x + 1) * TILE_SIZE, (fillTexture.y + 1) * TILE_SIZE);
	quad[3].texCoords = sf::Vector2f(fillTexture.x * TILE_SIZE, (fillTexture.y + 1) * TILE_SIZE);
}


Level::~Level()
{
}
