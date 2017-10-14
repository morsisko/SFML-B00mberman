#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <memory>
#include "Player.h"
#include "NetGame.h"

class Server
{
private:
	static const int MAX_PLAYERS = 2;
	sf::TcpListener listener;
	sf::Clock clock;
	sf::SocketSelector selector;
	std::unique_ptr<NetGame> game;
	std::vector<std::unique_ptr<Player>> sockets;
	void checkForIncommingConnections();
	void checkForIncommingPackets();
	void processPackets();
public:
	Server();
	void start(int port);
	~Server();


};
