#pragma once
#include <memory>
#include "State.h"
#include "AssetsManager.h"

class State;
class GameStateManager
{
private:
	std::unique_ptr<State> currentState;
	AssetsManager assetsManager;
public:
	GameStateManager();
	void handleEvent(const sf::Event &event);
	void render();
	void update(const sf::Time &deltaTime);
	void setState(std::unique_ptr<State> state);
	AssetsManager& getAssets();
	~GameStateManager();
};

