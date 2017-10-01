#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"

enum PlayerAppearance
{
	GREEN_ORC = 0,
	BLUE_ORC

};

enum Direction
{
	NONE = 0,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class AbstractPlayer : public sf::Drawable
{
private:
	static const int GREEN_ORC_TILE_ROW = 1;
	static const int BLUE_ORC_TILE_ROW = 3;

	int getTileRowFromApperance(PlayerAppearance playerAppearance);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::Sprite sprite;
	Level& level;
	float velocity = 150.0f;

	bool collide(const sf::FloatRect& position);
public:
	AbstractPlayer(sf::Texture& texture, Level& level, sf::Vector2i position, PlayerAppearance playerAppearance);
	virtual void update(const sf::Time& deltaTime) = 0;
	~AbstractPlayer();
};

