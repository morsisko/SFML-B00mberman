#pragma once
#include <SFML\Network.hpp>
#include <deque>

class Player : public sf::TcpSocket
{
private:
	std::deque<sf::Packet> packetsQueue;

public:
	Player();
	void sendPacket(sf::Packet& packet);
	void processPackets();
	~Player();
};

