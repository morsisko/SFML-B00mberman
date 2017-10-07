#pragma once
#include <SFML\Graphics.hpp>
#include "GameStateManager.h"
#include "SplashScreen.h"

enum ServerPackets
{
	INIT_GAME = 0
};

enum ClientPackets
{
	MOVE = 0
};

class Game
{
private:
	GameStateManager manager;
	sf::RenderWindow window;
	sf::Clock frameClock;

public:
	static const int TILE_SIZE = 16;
	static const int DISPLAY_TILE_SIZE = 48;

	Game(sf::String title, int width, int height);
	void start();
	~Game();
};

