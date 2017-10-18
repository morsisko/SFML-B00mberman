#pragma once
#include "AbstractPlayer.h"
#include "NetworkManager.h"
#include <iostream>
#include <SFML\Network.hpp>
#include "Game.h"

class LocalPlayer :
	public AbstractPlayer
{
private:
	Direction lastKeyPressed = NONE;
	NetworkManager& networkManager;
	void tryToMoveAndSchedulePosition(Direction direction);
	void sendCurrentScheduledPosition();
	void sendBombRequest();
public:
	LocalPlayer(sf::Texture& texture, Level& level, NetworkManager& networkManager, sf::Vector2i position, PlayerAppearance playerAppearance);
	virtual void update(const sf::Time& deltaTime) override;
	void handleEvent(const sf::Event& event);
	void tp(int x, int y);
	~LocalPlayer();
};

