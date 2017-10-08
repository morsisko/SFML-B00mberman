#pragma once
#include "AbstractPlayer.h"
class NetPlayer :
	public AbstractPlayer
{
private:
	std::deque<sf::Vector2i> positionQueue;

	Direction getDirectionFromDeltaPosition(sf::Vector2i nextPosition);
	Direction tryGetAndSchedulePosition();
public:
	NetPlayer(sf::Texture& texture, Level& level, sf::Vector2i position, PlayerAppearance playerAppearance);
	virtual void update(const sf::Time& deltaTime) override;
	void move(sf::Vector2i nextPosition);
	~NetPlayer();
};

