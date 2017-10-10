#pragma once
#include <SFML\Graphics.hpp>
#include <array>
#include "Animation.h"

class Bomb :
	public sf::Drawable
{
private:
	static constexpr std::array<int, 6> ANIMATION_INDEXES = { 4, 5, 6, 7, 8, 9 };
	static const int ANIMATION_ROW = 5;
	static const int FRAME_TIME = 600;
	Animation animation;
	sf::Sprite sprite;
	int id;

	void initAnimation();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Bomb(sf::Texture& texture, int x, int y, int id);
	void update(const sf::Time& deltaTime);
	~Bomb();
};

