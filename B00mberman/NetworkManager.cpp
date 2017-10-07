#include "NetworkManager.h"


NetworkManager::NetworkManager(std::unique_ptr<sf::TcpSocket> server) : server(std::move(server))
{
}

void NetworkManager::send(sf::Packet & packet)
{
	packets.push_back(packet);
}

void NetworkManager::process()
{
	if (packets.empty())
		return;

	if (server->send(packets.front()) == sf::Socket::Done)
		packets.pop_front();
}

NetworkManager::~NetworkManager()
{
}
