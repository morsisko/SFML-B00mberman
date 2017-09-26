#pragma once
#include "State.h"
#include "Level.h"
#include <array>

class GameState :
	public State
{
private:
	Level level;
public:
	GameState(GameStateManager* manager, sf::RenderWindow* window);
	virtual void handleEvent(const sf::Event &event);
	virtual void render();
	virtual void update(const sf::Time &deltaTime);
	~GameState();
};

