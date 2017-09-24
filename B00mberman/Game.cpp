#include "Game.h"



Game::Game(sf::String title, int width, int height) : window(sf::VideoMode(width, height), title)
{
	window.setFramerateLimit(60);
	manager.setState(std::make_unique<SplashScreen>(&manager, &window, 1000));
}

void Game::start()
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			manager.handleEvent(event);
		}

		window.clear();
		manager.update(frameClock.restart());
		manager.render();
		window.display();
	}
}


Game::~Game()
{
}
