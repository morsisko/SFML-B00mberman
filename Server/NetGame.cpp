#include "NetGame.h"
#include "Player.h"


Player * NetGame::getOpponent(Player * player)
{
	if (players.front() == player)
		return players.back();

	return players.front();
}

void NetGame::handleMove(sf::Packet & packet, Player * sender)
{
	sf::Uint8 x;
	sf::Uint8 y;

	packet >> x >> y;

	sender->setPosition(sf::Vector2i(x, y));

	getOpponent(sender)->sendOpponentMove(sender);

}

void NetGame::handleBombRequest(sf::Packet & packet, Player * sender)
{
	bombs.push_back(ServerBomb(currentBombId, sender->getPosition(), sender));
	currentBombId++;

	for (auto& player : players)
		player->sendBombInfo(bombs.back());
}

NetGame::NetGame(Player* firstPlayer, Player* secondPlayer) : players{firstPlayer, secondPlayer}
{
	logicArray = { {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		} };

	firstPlayer->setAppearance(BLUE_ORC);
	firstPlayer->setPosition(sf::Vector2i(1, 1));

	secondPlayer->setAppearance(GREEN_ORC);
	secondPlayer->setPosition(sf::Vector2i(2, 1));

	for (auto& player : players)
		player->sendInitPacket(logicArray, getOpponent(player));

}

void NetGame::update(const sf::Time & deltaTime)
{
	for (auto& bomb : bombs)
	{
		bomb.update(deltaTime);
		if (bomb.isOverTime())
		{
			bomb.explode();

			for (auto& player : players)
				player->sendExplosionInfo(bomb);
		}
	}

	bombs.erase(std::remove_if(bombs.begin(), bombs.end(), [](ServerBomb &bomb) { return bomb.isExploded(); }), bombs.end());
}

void NetGame::handlePacket(sf::Packet& packet, Player* sender)
{
	sf::Uint8 headerNumber;
	packet >> headerNumber;

	ClientPackets clientPacketHeader = static_cast<ClientPackets>(headerNumber);

	if (clientPacketHeader == ClientPackets::MOVE)
		handleMove(packet, sender);

	else if (clientPacketHeader == ClientPackets::REQUEST_BOMB)
		handleBombRequest(packet, sender);

	else
		std::cout << "[ERROR]: Unknown packet header " << static_cast<int>(headerNumber) << std::endl;
}


NetGame::~NetGame()
{
}
