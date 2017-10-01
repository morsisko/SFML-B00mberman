#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Animation.h"

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
	static const int FRAME_TIME = 200;
	static const int GREEN_ORC_TILE_ROW = 1;
	static const int BLUE_ORC_TILE_ROW = 3;
	static constexpr std::array<int, 3> FRONT_ANIMATION_INDEXES = { 1, 2, 3 };
	static constexpr std::array<int, 3> BOTTOM_ANIMATION_INDEXES = { 0, 8, 9 };
	static constexpr std::array<int, 4> SIDE_ANIMATION_INDEXES = { 4, 5, 6, 7 };

	Animation upAnimation;
	Animation downAnimation;
	Animation leftAnimation;
	Animation rightAnimation;

	PlayerAppearance playerAppearance;

	int getTileRowFromApperance(PlayerAppearance playerAppearance);
	void initializeAnimations();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::Sprite sprite;
	Level& level;
	float velocity = 150.0f;
	Direction direction = NONE;
	Animation& currentAnimation;

	void setAnimationFromDirection();
	bool collide(const sf::FloatRect& position);
public:
	AbstractPlayer(sf::Texture& texture, Level& level, sf::Vector2i position, PlayerAppearance playerAppearance);
	virtual void update(const sf::Time& deltaTime) = 0;
	~AbstractPlayer();
};

