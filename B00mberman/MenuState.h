#pragma once
#include "State.h"
#include "SimpleButton.h"
#include "GameState.h"
#include <iostream>
class MenuState :
	public State
{
private:
	sf::Font font;
	SimpleButton playBtn;
	virtual void play();
public:
	MenuState(GameStateManager* manager, sf::RenderWindow* window);
	virtual void handleEvent(const sf::Event &event);
	virtual void render();
	virtual void update(const sf::Time &deltaTime);
	~MenuState();
};

