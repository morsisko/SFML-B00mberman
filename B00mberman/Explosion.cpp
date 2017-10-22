#include "Explosion.h"
#include "Level.h"
#include "Game.h"


void Explosion::initAnimation()
{
	for (auto& index : HEAD_ANIMATION_INDEXES)
		animation.addFrame(sf::IntRect(index.first * Game::TILE_SIZE, index.second * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));

	animation.reset(head);
}

Explosion::Direction Explosion::getDirectionByDeltaPosition(sf::Vector2i & delta)
{
	if (delta == sf::Vector2i(-1, 0))
		return LEFT;

	else if (delta == sf::Vector2i(0, -1))
		return TOP;

	else if (delta == sf::Vector2i(1, 0))
		return RIGHT;

	else if (delta == sf::Vector2i(0, 1))
		return DOWN;
}

void Explosion::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &texture;
	target.draw(vertex, states);

	target.draw(head);
}

sf::Vector2i Explosion::getTileByDirection(Direction direction, bool isLast)
{
	if (direction == TOP)
	{
		if (isLast)
			return sf::Vector2i(14, 0);

		return sf::Vector2i(14, 1);
	}

	else if (direction == DOWN)
	{
		if (isLast)
			return sf::Vector2i(14, 2);

		return sf::Vector2i(14, 1);
	}

	else if (direction == LEFT)
	{
		if (isLast)
			return sf::Vector2i(0, 5);

		return sf::Vector2i(1, 5);
	}

	else if (direction == RIGHT)
	{
		if (isLast)
			return sf::Vector2i(3, 5);

		return sf::Vector2i(1, 5);
	}
}

Explosion::Explosion(sf::Texture& texture, sf::Vector2i& position, int radius) : animation(FRAME_TIME, false), radius(radius), texture(texture)
{
	vertex.setPrimitiveType(sf::Quads);
	vertex.resize(radius * 4 * 4);
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

int Explosion::getRadius()
{
	return radius;
}

void Explosion::addExplosionTile(sf::Vector2i delta, sf::Vector2i position, bool isLast)
{
	sf::Vector2i fillTexture = getTileByDirection(getDirectionByDeltaPosition(delta), isLast);
	int x = position.x;
	int y = position.y;
	sf::Vertex* quad = &vertex[currentSize * 4];
	currentSize++;

	// define its 4 corners
	quad[0].position = sf::Vector2f(x * Game::DISPLAY_TILE_SIZE, y * Game::DISPLAY_TILE_SIZE);
	quad[1].position = sf::Vector2f((x + 1) * Game::DISPLAY_TILE_SIZE, y * Game::DISPLAY_TILE_SIZE);
	quad[2].position = sf::Vector2f((x + 1) * Game::DISPLAY_TILE_SIZE, (y + 1) * Game::DISPLAY_TILE_SIZE);
	quad[3].position = sf::Vector2f(x * Game::DISPLAY_TILE_SIZE, (y + 1) * Game::DISPLAY_TILE_SIZE);

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(fillTexture.x * Game::TILE_SIZE, fillTexture.y * Game::TILE_SIZE);
	quad[1].texCoords = sf::Vector2f((fillTexture.x + 1) * Game::TILE_SIZE, fillTexture.y * Game::TILE_SIZE);
	quad[2].texCoords = sf::Vector2f((fillTexture.x + 1) * Game::TILE_SIZE, (fillTexture.y + 1) * Game::TILE_SIZE);
	quad[3].texCoords = sf::Vector2f(fillTexture.x * Game::TILE_SIZE, (fillTexture.y + 1) * Game::TILE_SIZE);
}

sf::Vector2i Explosion::getPosition()
{
	sf::Vector2f position = head.getPosition();
	return Level::getLogicPositionFromRealPosition(position.x, position.y);
}


Explosion::~Explosion()
{
}
