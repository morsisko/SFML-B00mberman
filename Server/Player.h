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
	int bombExplosionRadius = 3;
	int maxBombs = 2;
	bool kill = false;

	void sendPacket(sf::Packet& packet);
public:
	Player();
	void processPackets();

	void sendInitPacket(std::array<std::array<int, NetGame::MAP_WIDTH>, NetGame::MAP_HEIGHT>& logicArray, Player* opponent);
	void sendOpponentMove(Player* opponent);
	void sendBombInfo(ServerBomb& bomb);
	void sendExplosionInfo(ServerBomb& bomb, std::vector<sf::Vector2i>& destroyedBlocks, bool gameEnd, bool draw);
	void sendTp(sf::Vector2i position);

	void setAppearance(PlayerAppearance appearance);
	void setPosition(sf::Vector2i position);
	void endLife();

	const sf::Vector2i& getPosition();
	int getBombExplosionRadius();
	int getMaxBombs();
	bool isKilled();

	std::string getName();
	~Player();
};

