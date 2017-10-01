#include "AbstractPlayer.h"
#include "Game.h"

int AbstractPlayer::getTileRowFromApperance(PlayerAppearance playerAppearance)
{
	if (playerAppearance == GREEN_ORC)
		return GREEN_ORC_TILE_ROW;

	else if (playerAppearance == BLUE_ORC)
		return BLUE_ORC_TILE_ROW;

	std::cout << "[ERROR]: Unknown player appearance" << std::endl;
	return 0;
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

AbstractPlayer::AbstractPlayer(sf::Texture & texture, Level & level, sf::Vector2i position, PlayerAppearance playerAppearance) : level(level)
{
	sprite.setTexture(texture);
	int textureRow = getTileRowFromApperance(playerAppearance);

	sprite.setTextureRect(sf::IntRect(Game::TILE_SIZE, textureRow * Game::TILE_SIZE, Game::TILE_SIZE, Game::TILE_SIZE));

	sprite.setPosition(level.getRealPositionFromLogicPosition(position.x, position.y));
	
	float scale = Game::DISPLAY_TILE_SIZE / static_cast<float>(Game::TILE_SIZE);
	sprite.setScale(scale, scale);
}

AbstractPlayer::~AbstractPlayer()
{
}
