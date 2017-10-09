#include "LocalPlayer.h"



void LocalPlayer::tryToMoveAndSchedulePosition(Direction direction)
{
	if (direction == NONE)
		return;

	sf::FloatRect globalBounds = sprite.getGlobalBounds();
	if (direction == RIGHT)
	{
		globalBounds.left += globalBounds.width;
	}
	else if (direction == LEFT)
	{
		globalBounds.left -= 1;
	}
	else if (direction == UP)
	{
		globalBounds.top -= 1;
	}
	else if (direction == DOWN)
	{
		globalBounds.top += globalBounds.height;
	}

	if (collide(globalBounds))
		return;

	sf::Vector2i nextPosition = level.getLogicPositionFromRealPosition(globalBounds.left, globalBounds.top);
	if (nextPosition != scheduledLogicPosition)
	{
		scheduledLogicPosition = nextPosition;
		sendCurrentScheduledPosition();
	}

	scheduledPosition = level.getRealPositionFromLogicPosition(nextPosition.x, nextPosition.y);
	this->direction = direction;
	setAnimationFromDirection();
}

void LocalPlayer::sendCurrentScheduledPosition()
{
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(ClientPackets::MOVE) << static_cast<sf::Uint8>(scheduledLogicPosition.x) << static_cast<sf::Uint8>(scheduledLogicPosition.y);

	networkManager.send(packet);
}

LocalPlayer::LocalPlayer(sf::Texture& texture, Level& level, NetworkManager& networkManager, sf::Vector2i position, PlayerAppearance playerAppearance) :
	AbstractPlayer(texture, level, position, playerAppearance),
	networkManager(networkManager)
{

}

void LocalPlayer::update(const sf::Time & deltaTime)
{
	if (direction == NONE)
	{
		tryToMoveAndSchedulePosition(lastKeyPressed);
	}
	
	AbstractPlayer::update(deltaTime);

}

void LocalPlayer::handleEvent(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::D)
			lastKeyPressed = RIGHT;

		else if (event.key.code == sf::Keyboard::A)
			lastKeyPressed = LEFT;

		else if (event.key.code == sf::Keyboard::S)
			lastKeyPressed = DOWN;

		else if (event.key.code == sf::Keyboard::W)
			lastKeyPressed = UP;
	}

	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::D && lastKeyPressed == RIGHT)
			lastKeyPressed = NONE;

		else if (event.key.code == sf::Keyboard::A && lastKeyPressed == LEFT)
			lastKeyPressed = NONE;

		else if (event.key.code == sf::Keyboard::S && lastKeyPressed == DOWN)
			lastKeyPressed = NONE;

		else if (event.key.code == sf::Keyboard::W && lastKeyPressed == UP)
			lastKeyPressed = NONE;

		else if (event.key.code == sf::Keyboard::Space)
			level.putBomb(sprite.getPosition());
	}

}


LocalPlayer::~LocalPlayer()
{
}
