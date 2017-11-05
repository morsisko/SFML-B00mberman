#include "Server.h"

void Server::checkForIncommingConnections()
{
	if (selector.isReady(listener))
	{
		std::unique_ptr<Player> player = std::make_unique<Player>();
		if (listener.accept(*player) != sf::Socket::Done)
			return;

		if (sockets.size() >= MAX_PLAYERS)
		{
			player->disconnect();
			return;
		}

		sockets.push_back(std::move(player));
		selector.add(*sockets.back());
		std::cout << "[LOG]: New player joined the server" << std::endl;

		if (sockets.size() == MAX_PLAYERS)
			game = std::make_unique<NetGame>(sockets.front().get(), sockets.back().get());
	}
}

void Server::checkForIncommingPackets()
{
	for (std::vector<std::unique_ptr<Player>>::iterator it = sockets.begin(); it != sockets.end();)
	{
		Player* player = it->get();

		if (selector.isReady(*player))
		{
			sf::Packet packet;
			sf::Socket::Status status = player->receive(packet);
			if (status == sf::Socket::Done)
			{
				if (game)
					game->handlePacket(packet, player);
			}
			else if (status == sf::Socket::Disconnected)
			{
				selector.remove(*player);
				it = sockets.erase(it);
				std::cout << "[LOG]: Socket disconnected" << std::endl;
				game.reset();
				continue;
			}

			else if (status != sf::Socket::Done)
			{
				std::cout << "[ERROR]: While reading packet" << std::endl;
			}

		}
			++it;
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
		}

		processPackets();

		if (game)
		{
			game->update(clock.restart());
			
			if (game->isGameEnd())
				game.reset();
		}
	}

}

Server::~Server()
{
}