#pragma once
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include <array>

class Explosion : public sf::Drawable
{
private:
	static constexpr std::array<std::pair<int, int>, 4> HEAD_ANIMATION_INDEXES = {std::make_pair(14, 3), std::make_pair(14, 4), std::make_pair(14, 5), std::make_pair(2, 5) };
	static constexpr int FRAME_TIME = 150;
	static constexpr int DISAPPEAR_TIME = 800;
	sf::Sprite head;
	Animation animation;
	int elapsedTime = 0;
	bool deleted = false;

	void initAnimation();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Explosion(sf::Texture& texture, sf::Vector2i& position);
	void update(const sf::Time& deltaTime);
	bool canDelete();
	~Explosion();
};

