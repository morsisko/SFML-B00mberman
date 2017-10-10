#include "Bomb.h"

ServerBomb::ServerBomb(int id, sf::Vector2i position) : id(id), position(position)
{

}

int ServerBomb::getId()
{
	return id;
}

const sf::Vector2i& ServerBomb::getPosition()
{
	return position;
}

ServerBomb::~ServerBomb()
{
}