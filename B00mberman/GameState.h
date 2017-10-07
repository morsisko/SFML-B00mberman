#pragma once
#include "State.h"
#include "Level.h"
#include "LocalPlayer.h"
#include "NetworkManager.h"
#include <array>

class GameState :
	public State
{
private:
	Level level;
	NetworkManager networkManager;
	LocalPlayer localPlayer;
	LocalPlayer netPlayer;
public:
	GameState(GameStateManager* manager, sf::RenderWindow* window, std::unique_ptr<sf::TcpSocket> server, std::array<std::array<int, Level::MAP_WIDTH>, Level::MAP_HEIGHT> &logicArray, 
			sf::Vector2i localPlayerPosition, PlayerAppearance localPlayerAppearance,
			sf::Vector2i netPlayerPosition, PlayerAppearance netPlayerAppearance);
	virtual void handleEvent(const sf::Event &event);
	virtual void render();
	virtual void update(const sf::Time &deltaTime);
	~GameState();
};

