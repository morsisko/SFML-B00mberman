#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <deque>

class NetworkManager
{
private:
	std::unique_ptr<sf::TcpSocket> server;
	std::deque<sf::Packet> packets;
public:
	NetworkManager(std::unique_ptr<sf::TcpSocket> server);
	void send(sf::Packet& packet);
	void process();
	bool recvPacket(sf::Packet& packet);
	~NetworkManager();
};

