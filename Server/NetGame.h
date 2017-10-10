#pragma once
#include <array>
#include "SFML\Network.hpp"
#include <iostream>
#include "Bomb.h"

enum ServerPackets
{
	INIT_GAME = 0,
	MOVE_ENEMY,
	PUT_BOMB
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
	DIRT
};

class Player;
class NetGame
{
public:
	static const int MAP_HEIGHT = 13;
	static const int MAP_WIDTH = 15;
	static const int MAX_IN_GAME = 2;

private:
	std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT> logicArray;
	std::array<Player*, MAX_IN_GAME> players;
	std::vector<ServerBomb> bombs;
	int currentBombId = 0;

	Player* getOpponent(Player* player);
	void handleMove(sf::Packet& packet, Player* sender);
	void handleBombRequest(sf::Packet& packet, Player* sender);
public:
	NetGame(Player* firstPlayer, Player* secondPlayer);
	void update(const sf::Time& deltaTime);
	void handlePacket(sf::Packet& packet, Player* sender);
	~NetGame();
};




