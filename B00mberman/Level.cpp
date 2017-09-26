#include "Level.h"



void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &texture;
	target.draw(vertex, states);
}

Level::Level(int x, int y) : size(x, y)
{
	texture.loadFromFile("assets/tile.png");

	vertex.setPrimitiveType(sf::Quads);
	vertex.resize(x * y * 4);
}

void Level::load(std::array<std::array<int, 15>, 13>& levelData)
{
	for (int i = 0; i < size.x; ++i)
	{
		for (int j = 0; j < size.y; ++j)
		{
			sf::Vertex* quad = &vertex[(i + j * size.x) * 4];

			int tile = levelData[j][i];
			sf::Vector2i fillTexture;
			if (tile == 1)
				fillTexture = sf::Vector2i(10, 5);
			else if (tile == 2)
				fillTexture = sf::Vector2i(10, 4);
			else
				fillTexture = sf::Vector2i(1, 0);

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * DISPLAY_TILE_SIZE, j * DISPLAY_TILE_SIZE);
			quad[1].position = sf::Vector2f((i + 1) * DISPLAY_TILE_SIZE, j * DISPLAY_TILE_SIZE);
			quad[2].position = sf::Vector2f((i + 1) * DISPLAY_TILE_SIZE, (j + 1) * DISPLAY_TILE_SIZE);
			quad[3].position = sf::Vector2f(i * DISPLAY_TILE_SIZE, (j + 1) * DISPLAY_TILE_SIZE);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(fillTexture.x * TILE_SIZE, fillTexture.y * TILE_SIZE);
			quad[1].texCoords = sf::Vector2f((fillTexture.x + 1) * TILE_SIZE, fillTexture.y * TILE_SIZE);
			quad[2].texCoords = sf::Vector2f((fillTexture.x + 1) * TILE_SIZE, (fillTexture.y + 1) * TILE_SIZE);
			quad[3].texCoords = sf::Vector2f(fillTexture.x * TILE_SIZE, (fillTexture.y + 1) * TILE_SIZE);
		}
	}
}


Level::~Level()
{
}
