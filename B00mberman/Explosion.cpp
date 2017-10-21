#include "Explosion.h"
#include "Level.h"
#include "Game.h"


void Explosion::initAnimation()
{
	for (auto& index : HEAD_ANIMATION_INDEXES)
		animation.addFrame(sf::IntRect(index.first * Game::TILE_SIZE, index.second * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));

	animation.reset(head);
}

void Explosion::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(head, states);
}

Explosion::Explosion(sf::Texture& texture, sf::Vector2i& position) : animation(FRAME_TIME, false)
{
	head.setTexture(texture);
	head.setPosition(Level::getRealPositionFromLogicPosition(position.x, position.y));
	float scale = Game::DISPLAY_TILE_SIZE / static_cast<float>(Game::TILE_SIZE);
	head.setScale(scale, scale);

	initAnimation();
}

void Explosion::update(const sf::Time & deltaTime)
{
	if (deleted)
		return;

	animation.update(deltaTime, head);
	elapsedTime += deltaTime.asMilliseconds();

	if (elapsedTime >= DISAPPEAR_TIME)
		deleted = true;
}

bool Explosion::canDelete()
{
	return deleted;
}


Explosion::~Explosion()
{
}
