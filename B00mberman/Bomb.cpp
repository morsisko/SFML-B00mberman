#include "..\Server\Bomb.h"
#include "Bomb.h"
#include "Game.h"


Bomb::Bomb(sf::Texture& texture, int x, int y, int id, int explosionRadius) : animation(FRAME_TIME, false),
	id(id),
	explosionRadius(explosionRadius)
{
	sprite.setTexture(texture);
	initAnimation();
	sprite.setPosition(x * Game::DISPLAY_TILE_SIZE, y * Game::DISPLAY_TILE_SIZE);
	
	float scale = Game::DISPLAY_TILE_SIZE / static_cast<float>(Game::TILE_SIZE);

	sprite.setScale(scale, scale);
}

void Bomb::update(const sf::Time & deltaTime)
{
	animation.update(deltaTime, sprite);
}

int Bomb::getId()
{
	return id;
}

int Bomb::getRadius()
{
	return explosionRadius;
}

const sf::Vector2f Bomb::getPosition()
{
	return sprite.getPosition();
}

void Bomb::initAnimation()
{
	for (auto& index : ANIMATION_INDEXES)
		animation.addFrame(sf::IntRect(index * Game::TILE_SIZE, ANIMATION_ROW * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));

	animation.reset(sprite);
}

void Bomb::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Bomb::~Bomb()
{
}
