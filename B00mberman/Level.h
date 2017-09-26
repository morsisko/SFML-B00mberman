#pragma once
#include "SFML\Graphics.hpp"
#include <array>

class Level : public sf::Drawable
{
private:
	sf::VertexArray vertex;
	sf::Texture texture;
	sf::Vector2i size;

	const int TILE_SIZE = 16;
	const int DISPLAY_TILE_SIZE = 48;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Level(int x, int y);
	void load(std::array<std::array<int, 15>, 13>& levelData);
	~Level();
};

