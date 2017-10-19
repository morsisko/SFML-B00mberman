#include "NetGame.h"
#include "Player.h"


bool NetGame::isValidPosition(int x, int y)
{
	return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
}

bool NetGame::canWalkOnTile(int x, int y)
{
	return isValidPosition(x, y) && (logicArray[y][x] == DIRT || logicArray[y][x] == GRASS);
}

bool NetGame::isProtected(int x, int y)
{
	for (auto& position : protectedPositions)
		if (position.x == x && position.y == y)
			return true;

	return false;
}

std::vector<sf::Vector2i> NetGame::gatherFreePositions()
{
	std::vector<sf::Vector2i> results;

	for (int x = 0; x < MAP_WIDTH; ++x)
		for (int y = 0; y < MAP_HEIGHT; ++y)
			if (logicArray[y][x] == GRASS && !isProtected(x, y))
				results.push_back(sf::Vector2i(x, y));

	return results;
}

void NetGame::shufflePositions(std::vector<sf::Vector2i>& v)
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(v.begin(), v.end(), g);
}

int NetGame::calculateBombAmountByPlayer(Player * player)
{
	return std::count_if(bombs.begin(), bombs.end(), [player](ServerBomb& bomb) {return bomb.getOwner() == player; });
}

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

	if (!canWalkOnTile(x, y))
		sender->sendTp(sender->getPosition());

	else
	{
		sender->setPosition(sf::Vector2i(x, y));
		getOpponent(sender)->sendOpponentMove(sender);
	}


}

void NetGame::handleBombRequest(sf::Packet & packet, Player * sender)
{
	if (calculateBombAmountByPlayer(sender) >= sender->getMaxBombs())
		return;

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

	std::vector<sf::Vector2i> freePositions = gatherFreePositions();
	std::cout << "[LOG]: Gathered positions: " << freePositions.size() << std::endl;

	shufflePositions(freePositions);

	for (int i = 0; i < MAX_BOXES && i < freePositions.size(); ++i)
	{
		sf::Vector2i &currentPosition = freePositions.at(i);
		logicArray[currentPosition.y][currentPosition.x] = static_cast<int>(TileType::BOX);
	}

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
