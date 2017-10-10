#pragma once
#include <SFML\Network.hpp>

class ServerBomb
{
	sf::Vector2i position;
	int id;
public:
	ServerBomb(int id, sf::Vector2i position);
	int getId();
	const sf::Vector2i& getPosition();
	~ServerBomb();

};