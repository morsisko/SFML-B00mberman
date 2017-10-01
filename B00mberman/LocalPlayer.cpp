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
	scheduledPosition = level.getRealPositionFromLogicPosition(nextPosition.x, nextPosition.y);
	this->direction = direction;
}

LocalPlayer::LocalPlayer(sf::Texture& texture, Level& level, sf::Vector2i position, PlayerAppearance playerAppearance) :
	AbstractPlayer(texture, level, position, playerAppearance)
{
}

void LocalPlayer::update(const sf::Time & deltaTime)
{
	handleInput();

	if (direction == NONE)
		return;

	float deltaAsSeconds = deltaTime.asSeconds();
	sf::Vector2f position = sprite.getPosition();

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


LocalPlayer::~LocalPlayer()
{
}