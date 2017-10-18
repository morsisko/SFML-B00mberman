#pragma once
#include <SFML\Network.hpp>
#include "NetGame.h"
#include "Bomb.h"
#include <deque>

enum PlayerAppearance
{
	GREEN_ORC = 0,
	BLUE_ORC

};

class Player : public sf::TcpSocket
{
private:
	std::deque<sf::Packet> packetsQueue;
	PlayerAppearance appearance;
	sf::Vector2i position;
	int bombExplosionRadius = 1;
	int maxBombs = 2;

	void sendPacket(sf::Packet& packet);
public:
	Player();
	void processPackets();

	void sendInitPacket(std::array<std::array<int, NetGame::MAP_WIDTH>, NetGame::MAP_HEIGHT>& logicArray, Player* opponent);
	void sendOpponentMove(Player* opponent);
	void sendBombInfo(ServerBomb& bomb);
	void sendExplosionInfo(ServerBomb& bomb);
	void sendTp(sf::Vector2i position);

	void setAppearance(PlayerAppearance appearance);
	void setPosition(sf::Vector2i position);
	const sf::Vector2i& getPosition();

	int getBombExplosionRadius();
	int getMaxBombs();

	~Player();
};

