#pragma once
#include <SFML\Graphics.hpp>
#include "GameStateManager.h"

class GameStateManager;
class State
{
protected:
	GameStateManager* manager;
	sf::RenderWindow* window;
public:
	State(GameStateManager* manager, sf::RenderWindow* window);
	virtual void handleEvent(const sf::Event &event) = 0;
	virtual void render() = 0;
	virtual void update(const sf::Time &deltaTime) = 0;
	virtual ~State() = 0;
};

