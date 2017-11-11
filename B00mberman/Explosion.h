#pragma once
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include <array>

class Explosion : public sf::Drawable
{
private:
	enum Direction
	{
		LEFT = 0,
		TOP,
		RIGHT,
		DOWN
	};

	static constexpr std::array<std::pair<int, int>, 4> HEAD_ANIMATION_INDEXES = {std::make_pair(14, 3), std::make_pair(14, 4), std::make_pair(14, 5), std::make_pair(2, 5) };
	static constexpr int FRAME_TIME = 150;
	static constexpr int DISAPPEAR_TIME = 800;
	sf::Texture& texture;
	sf::Sprite head;
	Animation animation;
	sf::VertexArray vertex;
	int elapsedTime = 0;
	int radius;
	int currentSize = 0;
	bool deleted = false;

	void initAnimation();
	Direction getDirectionByDeltaPosition(sf::Vector2i& delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getTileByDirection(Direction direction, bool isLast);
public:
	Explosion(sf::Texture& texture, sf::Vector2i& position, int radius);
	void update(const sf::Time& deltaTime);
	bool canDelete();
	int getRadius();
	void addExplosionTile(sf::Vector2i delta, sf::Vector2i position, bool isLast);
	sf::Vector2i getPosition();
	void shrinkArray();
	~Explosion();
};

