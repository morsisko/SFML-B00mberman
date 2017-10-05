#include "Server.h"

void Server::checkForIncommingConnections()
{
	if (selector.isReady(listener))
	{
		std::unique_ptr<Player> player = std::make_unique<Player>();
		if (listener.accept(*player) != sf::Socket::Done)
			return;

		if (sockets.size() >= 2)
		{
			player->disconnect();
			return;
		}

		sockets.push_back(std::move(player));
		selector.add(*sockets.back());
	}
}

void Server::checkForIncommingPackets()
{
	for (std::vector<std::unique_ptr<Player>>::iterator it = sockets.begin(); it != sockets.end();)
	{
		Player& player = **it;

		if (selector.isReady(player))
		{
			sf::Packet packet;
			sf::Socket::Status status = player.receive(packet);
			if (status == sf::Socket::Disconnected)
			{
				selector.remove(player);
				sockets.erase(it);
				std::cout << "Socket disconnected" << std::endl;
			}

			else if (status != sf::Socket::Done)
			{
				std::cout << "Error while reading packet" << std::endl;
				++it;
				continue;
			}

			++it;
		}
	}
}

void Server::processPackets()
{
	for (auto& socket : sockets)
		socket->processPackets();
}

Server::Server()
{
}

void Server::start(int port)
{
	listener.listen(port);
	selector.add(listener);

	while (true)
	{
		if (selector.wait(sf::milliseconds(10)))
		{
			checkForIncommingConnections();
			checkForIncommingPackets();
			processPackets();
		}
	}

}

Server::~Server()
{
}