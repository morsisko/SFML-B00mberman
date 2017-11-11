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

bool NetGame::isDraw()
{
	if (!gameEnd)
		return false;

	bool draw = true;

	for (auto& player : players)
		if (!player->isKilled())
			draw = false;

	return draw;
}

void NetGame::tryKillPlayersOnPosition(int x, int y)
{
	for (auto& player : players)
	{
		sf::Vector2i playerPosition = player->getPosition();

		if (playerPosition.x == x && playerPosition.y == y)
		{
			player->endLife();
			gameEnd = true;
		}
	}
}

TileType NetGame::getTileTypeAt(int x, int y)
{
	if (!isValidPosition(x, y))
		return SPACE_BLOCK;

	return static_cast<TileType>(logicArray[y][x]);
}

void NetGame::setTileTypeAt(int x, int y, TileType tile)
{
	if (!isValidPosition(x, y))
		return;

	logicArray[y][x] = static_cast<int>(tile);
}

std::vector<sf::Vector2i> NetGame::affectExplosion(ServerBomb & bomb)
{
	bomb.explode();
	std::vector<sf::Vector2i> explodedBlocks;

	sf::Vector2i bombPosition = bomb.getPosition();

	tryKillPlayersOnPosition(bombPosition.x, bombPosition.y);

	for (auto& direction : directions)
	{
		sf::Vector2i position = bomb.getPosition() + direction;
		for (int i = 0; i != bomb.getExplosionRadius() && isValidPosition(position.x, position.y); ++i, position += direction)
		{
			TileType tileType = getTileTypeAt(position.x, position.y);
			if (tileType == SPACE_BLOCK || tileType == BORDER_BLOCK)
				break;

			if (tileType == BOX)
			{
				explodedBlocks.push_back(position);
				setTileTypeAt(position.x, position.y, DIRT);
				break;
			}

			tryKillPlayersOnPosition(position.x, position.y);
		}
	}

	return explodedBlocks;
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

	std::cout << "[LOG]: Created new game instance" << std::endl;

	firstPlayer->setAppearance(BLUE_ORC);
	firstPlayer->setPosition(sf::Vector2i(1, 1));

	secondPlayer->setAppearance(GREEN_ORC);
	secondPlayer->setPosition(sf::Vector2i(13, 11));

	std::vector<sf::Vector2i> freePositions = gatherFreePositions();
	std::cout << "[LOG]: Gathered positions: " << freePositions.size() << std::endl;

	shufflePositions(freePositions);

	for (int i = 0; i < MAX_BOXES && i < freePositions.size(); ++i)
	{
		sf::Vector2i &currentPosition = freePositions.at(i);
		setTileTypeAt(currentPosition.x, currentPosition.y, TileType::BOX);
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
			std::vector<sf::Vector2i> destroyedBlocks = affectExplosion(bomb);

			for (auto& player : players)
				player->sendExplosionInfo(bomb, destroyedBlocks, gameEnd, isDraw());
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

bool NetGame::isGameEnd()
{
	return gameEnd;
}


NetGame::~NetGame()
{
}
