#include "NetPlayer.h"


Direction NetPlayer::getDirectionFromDeltaPosition(sf::Vector2i nextPosition)
{
	sf::Vector2f realPosition = sprite.getPosition();

	nextPosition -= level.getLogicPositionFromRealPosition(realPosition.x, realPosition.y);

	if (nextPosition.x >= 1)
		return RIGHT;
	else if (nextPosition.x <= -1)
		return LEFT;
	else if (nextPosition.y >= 1)
		return DOWN;
	else if (nextPosition.y <= -1)
		return UP;

	return NONE;
}

Direction NetPlayer::tryGetAndSchedulePosition()
{
	if (positionQueue.empty())
		return NONE;

	sf::Vector2i toSchedule = positionQueue.front();
	positionQueue.pop_front();

	scheduledLogicPosition = toSchedule;
	scheduledPosition = level.getRealPositionFromLogicPosition(toSchedule.x, toSchedule.y);
	return getDirectionFromDeltaPosition(toSchedule);
}

NetPlayer::NetPlayer(sf::Texture & texture, Level & level, sf::Vector2i position, PlayerAppearance playerAppearance) : AbstractPlayer(texture, level, position, playerAppearance)
{

}

void NetPlayer::update(const sf::Time & deltaTime)
{
	if (direction == NONE)
	{
		direction = tryGetAndSchedulePosition();
	}

	if (direction != NONE)
	{
		setAnimationFromDirection();
		currentAnimation->update(deltaTime, sprite);
		float deltaAsSeconds = deltaTime.asSeconds();
		moveWithCurrentDirection(deltaAsSeconds);
		if (checkIfScheduledPositionReached())
			direction = NONE;
	}
	else
	{
		currentAnimation->reset(sprite);
	}
}

void NetPlayer::move(sf::Vector2i nextPosition)
{
	positionQueue.push_back(nextPosition);
}

NetPlayer::~NetPlayer()
{
}
