#include "AbstractPlayer.h"
#include "Game.h"

Animation * AbstractPlayer::getAnimationFromDirection(Direction direction)
{
	if (direction == UP)
		return &upAnimation;

	else if (direction == DOWN)
		return &downAnimation;

	else if (direction == LEFT)
		return &leftAnimation;

	else if (direction == RIGHT)
		return &rightAnimation;

	return nullptr;
}

int AbstractPlayer::getTileRowFromApperance(PlayerAppearance playerAppearance)
{
	if (playerAppearance == GREEN_ORC)
		return GREEN_ORC_TILE_ROW;

	else if (playerAppearance == BLUE_ORC)
		return BLUE_ORC_TILE_ROW;

	std::cout << "[ERROR]: Unknown player appearance" << std::endl;
	return 0;
}

void AbstractPlayer::initializeAnimations()
{
	int tile = getTileRowFromApperance(playerAppearance);

	for (auto& frameNumber : FRONT_ANIMATION_INDEXES)
	{
		downAnimation.addFrame(sf::IntRect(Game::TILE_SIZE * frameNumber, tile * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));
	}

	for (auto& frameNumber : BOTTOM_ANIMATION_INDEXES)
	{
		upAnimation.addFrame(sf::IntRect(Game::TILE_SIZE * frameNumber, tile * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));
	}

	for (auto& frameNumber : SIDE_ANIMATION_INDEXES)
	{
		rightAnimation.addFrame(sf::IntRect(Game::TILE_SIZE * frameNumber, tile * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));
		leftAnimation.addFrame(sf::IntRect(Game::TILE_SIZE * (frameNumber + 1), tile * Game::TILE_SIZE, -Game::TILE_SIZE, Game::TILE_SIZE));
	}
}

void AbstractPlayer::moveWithCurrentDirection(float deltaTime)
{
	if (direction == RIGHT)
		sprite.move(velocity * deltaTime, 0);

	else if (direction == LEFT)
		sprite.move(-velocity * deltaTime, 0);

	else if (direction == UP)
		sprite.move(0, -velocity * deltaTime);

	else if (direction == DOWN)
		sprite.move(0, velocity * deltaTime);
}

bool AbstractPlayer::checkIfScheduledPositionReached()
{
	sf::Vector2f position = sprite.getPosition();

	if (direction == RIGHT)
	{
		if (position.x >= scheduledPosition.x)
		{
			sprite.setPosition(scheduledPosition.x, position.y);
			return true;
		}
	}
	else if (direction == LEFT)
	{
		if (position.x <= scheduledPosition.x)
		{
			sprite.setPosition(scheduledPosition.x, position.y);
			return true;
		}
	}
	else if (direction == UP)
	{

		if (position.y <= scheduledPosition.y)
		{
			sprite.setPosition(position.x, scheduledPosition.y);
			return true;
		}
	}
	else if (direction == DOWN)
	{
		if (position.y >= scheduledPosition.y)
		{
			sprite.setPosition(position.x, scheduledPosition.y);
			return true;
		}
	}

	return false;
}

void AbstractPlayer::setAnimationFromDirection()
{
	Animation* newAnimation = getAnimationFromDirection(direction);

	if (newAnimation != currentAnimation)
	{
		currentAnimation = newAnimation;
		currentAnimation->reset(sprite);
	}
}

void AbstractPlayer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

bool AbstractPlayer::collide(const sf::FloatRect & position)
{
	return (level.isPointCollidable(position.left, position.top) ||
		level.isPointCollidable(position.left + position.width - 1, position.top) ||
		level.isPointCollidable(position.left + position.width - 1, position.top + position.height - 1) ||
		level.isPointCollidable(position.left, position.top + position.height - 1));
}

AbstractPlayer::AbstractPlayer(sf::Texture & texture, Level & level, sf::Vector2i position, PlayerAppearance playerAppearance) : level(level),
	playerAppearance(playerAppearance),
	upAnimation(FRAME_TIME),
	downAnimation(FRAME_TIME),
	leftAnimation(FRAME_TIME),
	rightAnimation(FRAME_TIME),
	currentAnimation(&downAnimation)
{
	sprite.setTexture(texture);
	int textureRow = getTileRowFromApperance(playerAppearance);

	sprite.setTextureRect(sf::IntRect(Game::TILE_SIZE, textureRow * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));

	sprite.setPosition(level.getRealPositionFromLogicPosition(position.x, position.y));
	
	float scale = Game::DISPLAY_TILE_SIZE / static_cast<float>(Game::TILE_SIZE);
	sprite.setScale(scale, scale);

	initializeAnimations();
}

void AbstractPlayer::update(const sf::Time & deltaTime)
{
	if (direction != NONE)
	{
		setAnimationFromDirection();
		currentAnimation->update(deltaTime, sprite);
		moveWithCurrentDirection(deltaTime.asSeconds());
		if (checkIfScheduledPositionReached())
			direction = NONE;
	}
	else
	{
		currentAnimation->reset(sprite);
	}
}

AbstractPlayer::~AbstractPlayer()
{
}
