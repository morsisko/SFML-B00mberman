#pragma once
#include "State.h"
class GameState :
	public State
{
public:
	GameState(GameStateManager* manager, sf::RenderWindow* window);
	virtual void handleEvent(const sf::Event &event);
	virtual void render();
	virtual void update(const sf::Time &deltaTime);
	~GameState();
};

