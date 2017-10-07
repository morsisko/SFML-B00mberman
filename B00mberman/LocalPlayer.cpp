#include "LocalPlayer.h"



void LocalPlayer::tryToMoveAndSchedulePosition(Direction direction)
{
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
	{
		return;
	}

	sf::Vector2i nextPosition = level.getLogicPositionFromRealPosition(globalBounds.left, globalBounds.top);
	if (nextPosition != scheduledLogicPosition)
	{
		scheduledLogicPosition = nextPosition;
		sendCurrentScheduledPosition();
	}

	scheduledPosition = level.getRealPositionFromLogicPosition(nextPosition.x, nextPosition.y);

	Direction oldDirection = this->direction;
	this->direction = direction;
	if (oldDirection != direction)
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
	handleInput();

	if (direction == NONE)
	{
		currentAnimation->reset(sprite);
	}

	sf::Vector2f position = sprite.getPosition(); // TODO: Split that position check

	float deltaAsSeconds = deltaTime.asSeconds();

	if (direction == RIGHT)
	{
		sprite.move(velocity * deltaAsSeconds, 0);
		
		if (position.x >= scheduledPosition.x)
		{
			sprite.setPosition(scheduledPosition.x, position.y);
			direction = NONE;
		}
	}
	else if (direction == LEFT)
	{
		sprite.move(-velocity * deltaAsSeconds, 0);

		if (position.x <= scheduledPosition.x)
		{
			sprite.setPosition(scheduledPosition.x, position.y);
			direction = NONE;
		}
	}
	else if (direction == UP)
	{
		sprite.move(0, -velocity * deltaAsSeconds);

		if (position.y <= scheduledPosition.y)
		{
			sprite.setPosition(position.x, scheduledPosition.y);
			direction = NONE;
		}
	}
	else if (direction == DOWN)
	{
		sprite.move(0, velocity * deltaAsSeconds);

		if (position.y >= scheduledPosition.y)
		{
			sprite.setPosition(position.x, scheduledPosition.y);
			direction = NONE;
		}
	}

	currentAnimation->update(deltaTime, sprite);

}

void LocalPlayer::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		tryToMoveAndSchedulePosition(RIGHT);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		tryToMoveAndSchedulePosition(LEFT);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		tryToMoveAndSchedulePosition(UP);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		tryToMoveAndSchedulePosition(DOWN);
	}
}

void LocalPlayer::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyReleased)
		return;

	if (event.key.code == sf::Keyboard::Space)
		level.putBomb(sprite.getPosition());
}


LocalPlayer::~LocalPlayer()
{
}
