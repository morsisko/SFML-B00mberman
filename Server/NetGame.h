#pragma once
#include <array>
#include "SFML\Network.hpp"
#include <iostream>
#include "Bomb.h"
#include <algorithm>
#include <random>

enum ServerPackets
{
	INIT_GAME = 0,
	MOVE_ENEMY,
	PUT_BOMB,
	EXPLODE,
	TP
};

enum ClientPackets
{
	MOVE = 0,
	REQUEST_BOMB
};

enum TileType
{
	GRASS = 0,
	BORDER_BLOCK,
	SPACE_BLOCK,
	DIRT,
	BOX
};

enum PlayerGameState
{
	DRAW = 0,
	WIN,
	LOSE
};

class Player;
class NetGame
{
public:
	static const int MAP_HEIGHT = 13;
	static const int MAP_WIDTH = 15;
	static const int MAX_IN_GAME = 2;
	static const int MAX_BOXES = 50;
	const std::array<sf::Vector2i, 6> protectedPositions = { sf::Vector2i(1, 1), sf::Vector2i(2, 1), sf::Vector2i(1, 2), sf::Vector2i(13, 11), sf::Vector2i(13, 10), sf::Vector2i(12, 11)};
	const std::array<sf::Vector2i, 4> directions = { sf::Vector2i(-1, 0), sf::Vector2i(0, -1), sf::Vector2i(1, 0), sf::Vector2i(0, 1) };

private:
	std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT> logicArray;
	std::array<Player*, MAX_IN_GAME> players;
	std::vector<ServerBomb> bombs;
	int currentBombId = 0;
	bool gameEnd = false;

	bool isValidPosition(int x, int y);
	bool canWalkOnTile(int x, int y);
	bool isProtected(int x, int y);
	bool isDraw();
	void tryKillPlayersOnPosition(int x, int y);
	TileType getTileTypeAt(int x, int y);
	void setTileTypeAt(int x, int y, TileType tile);
	std::vector<sf::Vector2i> affectExplosion(ServerBomb& bomb);
	std::vector<sf::Vector2i> gatherFreePositions();
	void shufflePositions(std::vector<sf::Vector2i> &v);
	int calculateBombAmountByPlayer(Player* player);
	Player* getOpponent(Player* player);
	void handleMove(sf::Packet& packet, Player* sender);
	void handleBombRequest(sf::Packet& packet, Player* sender);
public:
	NetGame(Player* firstPlayer, Player* secondPlayer);
	void update(const sf::Time& deltaTime);
	void handlePacket(sf::Packet& packet, Player* sender);
	bool isGameEnd();
	~NetGame();
};




