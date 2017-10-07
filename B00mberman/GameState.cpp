#include "GameState.h"

GameState::GameState(GameStateManager* manager, sf::RenderWindow* window, std::unique_ptr<sf::TcpSocket> server, std::array<std::array<int, Level::MAP_WIDTH>, Level::MAP_HEIGHT> &logicArray,
	sf::Vector2i localPlayerPosition, PlayerAppearance localPlayerAppearance, 
	sf::Vector2i netPlayerPosition, PlayerAppearance netPlayerAppearance) : State(manager, window),
	level(manager->getAssets().getTileTexture(), logicArray),
	networkManager(std::move(server)),
	localPlayer(manager->getAssets().getTileTexture(), level, networkManager, localPlayerPosition, localPlayerAppearance),
	netPlayer(manager->getAssets().getTileTexture(), level, networkManager, netPlayerPosition, netPlayerAppearance)
{
	;
}

void GameState::handleEvent(const sf::Event & event)
{
	localPlayer.handleEvent(event);
}

void GameState::render()
{
	window->draw(level);
	window->draw(localPlayer);
	window->draw(netPlayer);
}

void GameState::update(const sf::Time & deltaTime)
{
	localPlayer.update(deltaTime);
	level.update(deltaTime);
	networkManager.process();
}


GameState::~GameState()
{
}
