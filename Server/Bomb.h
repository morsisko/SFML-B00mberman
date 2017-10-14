#pragma once
#include <SFML\Network.hpp>

class Player;
class ServerBomb
{
	static const int EXPLOSION_AFTER = 4000;
	sf::Vector2i position;
	int id;
	int explosionRadius;
	int aliveTime = 0;
	bool exploded = false;
	Player* owner;
public:
	ServerBomb(int id, sf::Vector2i position, Player* owner);
	int getId();
	int getExplosionRadius();
	const sf::Vector2i& getPosition();
	void update(const sf::Time& deltaTime);
	Player* getOwner();
	void explode();
	bool isOverTime();
	bool isExploded();
	~ServerBomb();

};