#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <memory>
#include "Player.h"

class Server
{
private:
	static const int MAX_PLAYERS = 2;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<std::unique_ptr<Player>> sockets;
	void checkForIncommingConnections();
	void checkForIncommingPackets();
	void processPackets();
public:
	Server();
	void start(int port);
	~Server();


};
