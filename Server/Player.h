#pragma once
#include <SFML\Network.hpp>
#include "NetGame.h"
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
public:
	Player();
	void sendPacket(sf::Packet& packet);
	void processPackets();

	void sendInitPacket(std::array<std::array<int, NetGame::MAP_WIDTH>, NetGame::MAP_HEIGHT>& logicArray, Player* opponent);

	void setAppearance(PlayerAppearance appearance);
	void setPosition(sf::Vector2i position);
	const sf::Vector2i& getPosition(sf::Vector2i position);

	~Player();
};

