#pragma once
#include "AbstractPlayer.h"
#include <iostream>

class LocalPlayer :
	public AbstractPlayer
{
private:
	sf::Vector2f scheduledPosition;
	void tryToMoveAndSchedulePosition(Direction direction);

public:
	LocalPlayer(sf::Texture& texture, Level& level, sf::Vector2i position, PlayerAppearance playerAppearance);
	virtual void update(const sf::Time& deltaTime) override;
	void handleInput();
	void handleEvent(const sf::Event& event);
	~LocalPlayer();
};

