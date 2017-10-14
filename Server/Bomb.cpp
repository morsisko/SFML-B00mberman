#include "Bomb.h"
#include "Player.h"

ServerBomb::ServerBomb(int id, sf::Vector2i position, Player* owner) : id(id), position(position), owner(owner), explosionRadius(owner->getBombExplosionRadius())
{

}

int ServerBomb::getId()
{
	return id;
}

int ServerBomb::getExplosionRadius()
{
	return explosionRadius;
}

const sf::Vector2i& ServerBomb::getPosition()
{
	return position;
}

void ServerBomb::update(const sf::Time & deltaTime)
{
	aliveTime += deltaTime.asMilliseconds();
}

Player * ServerBomb::getOwner()
{
	return owner;
}

void ServerBomb::explode()
{
	exploded = true;
}

bool ServerBomb::isOverTime()
{
	return aliveTime >= EXPLOSION_AFTER;
}

bool ServerBomb::isExploded()
{
	return exploded;
}

ServerBomb::~ServerBomb()
{
}