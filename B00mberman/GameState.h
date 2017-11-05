#pragma once
#include "State.h"
#include "Level.h"
#include "LocalPlayer.h"
#include "NetPlayer.h"
#include "NetworkManager.h"
#include <array>

enum PlayerGameState
{
	DRAW = 0,
	WIN,
	LOSE
};

class GameState :
	public State
{
private:
	Level level;
	NetworkManager networkManager;
	LocalPlayer localPlayer;
	NetPlayer netPlayer;
	bool gameEnd = false;
	sf::Text endText;

	void handlePackets();
	void handleEnemyMove(sf::Packet& packet);
	void handlePutBomb(sf::Packet& packet);
	void handleExplosion(sf::Packet& packet);
	void handleTp(sf::Packet& packet);

	void endGame(PlayerGameState gameEndState);
	void gameEndEventHandle(const sf::Event &event);
public:
	GameState(GameStateManager* manager, sf::RenderWindow* window, std::unique_ptr<sf::TcpSocket> server, std::array<std::array<int, Level::MAP_WIDTH>, Level::MAP_HEIGHT> &logicArray, 
			sf::Vector2i localPlayerPosition, PlayerAppearance localPlayerAppearance,
			sf::Vector2i netPlayerPosition, PlayerAppearance netPlayerAppearance);
	virtual void handleEvent(const sf::Event &event);
	virtual void render();
	virtual void update(const sf::Time &deltaTime);
	~GameState();
};

