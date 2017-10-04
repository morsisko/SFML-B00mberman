#include "GameState.h"

GameState::GameState(GameStateManager* manager, sf::RenderWindow* window) : State(manager, window),
	level(manager->getAssets().getTileTexture()),
	localPlayer(manager->getAssets().getTileTexture(), level, sf::Vector2i(1, 1), PlayerAppearance::BLUE_ORC)
{
	std::array<std::array<int, 15>, 13> levelData{ { 
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
	level.load(levelData);
}

void GameState::handleEvent(const sf::Event & event)
{
	localPlayer.handleEvent(event);
}

void GameState::render()
{
	window->draw(level);
	window->draw(localPlayer);
}

void GameState::update(const sf::Time & deltaTime)
{
	localPlayer.update(deltaTime);
	level.update(deltaTime);
}


GameState::~GameState()
{
}
