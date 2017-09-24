#pragma once
#include "State.h"
#include <iostream>
#include "MenuState.h"

class SplashScreen :
	public State
{
private:
	sf::Texture splashTexture;
	sf::Sprite splashSprite;
	int elapsedTime = 0;
	int disappearTime = 0;

	virtual void onTimeOver();
public:
	SplashScreen(GameStateManager* manager, sf::RenderWindow* window, int miliseconds);
	virtual void handleEvent(const sf::Event &event);
	virtual void render();
	virtual void update(const sf::Time &deltaTime);
	virtual ~SplashScreen();
};

