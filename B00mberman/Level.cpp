#include "Level.h"
#include "Game.h"



void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &texture;
	target.draw(vertex, states);
	
	for (auto& bomb : bombs)
		target.draw(bomb);

	for (auto& explosion : explosions)
		target.draw(*explosion);
}

sf::Vector2i Level::getTileIndexFromTileType(TileType type)
{
	if (type == GRASS)
		return sf::Vector2i(1, 0);

	else if (type == BORDER_BLOCK)
		return sf::Vector2i(10, 5);

	else if (type == SPACE_BLOCK)
		return sf::Vector2i(10, 4);

	else if (type == BOX)
		return sf::Vector2i(9, 0);
	
	return sf::Vector2i(0, 0);
	
}

TileType Level::intToTileType(int type)
{
	return static_cast<TileType>(type);
}

Level::Level(sf::Texture &texture, std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& levelData) : texture(texture)
{
	vertex.setPrimitiveType(sf::Quads);
	vertex.resize(MAP_WIDTH * MAP_HEIGHT * 4);

	load(levelData);
}

void Level::load(std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& levelData)
{
	for (int i = 0; i < MAP_WIDTH; ++i)
	{
		for (int j = 0; j < MAP_HEIGHT; ++j)
		{
			TileType tileType = intToTileType(levelData[j][i]);
			setTileAsType(i, j, tileType);
		}
	}
}

void Level::initExplosion(Explosion * explosion)
{
	sf::Vector2i startPosition = explosion->getPosition();

	for (auto& direction : directions)
	{
		sf::Vector2i position = startPosition + direction;

		for (int i = 1; i <= explosion->getRadius() && isValidPosition(position.x, position.y); ++i, position += direction)
		{
			if (isBox(position.x, position.y))
			{
				explosion->addExplosionTile(direction, position, true);
				break;
			}

			else if (isLogicPointCollidable(position))
				break;

			explosion->addExplosionTile(direction, position, i == explosion->getRadius());

		}
	}

	explosion->shrinkArray();
}

bool Level::isValidPosition(int x, int y)
{
	return x < MAP_WIDTH && x >= 0 && y < MAP_HEIGHT && y >= 0;
}

bool Level::isPointCollidable(float x, float y)
{
	return isLogicPointCollidable(getLogicPositionFromRealPosition(x, y));
}

bool Level::isLogicPointCollidable(sf::Vector2i position)
{
	return !(isValidPosition(position.x, position.y) && logicArray[position.y][position.x] == GRASS || logicArray[position.y][position.x] == DIRT);
}

bool Level::isBox(int x, int y)
{
	if (!isValidPosition(x, y))
		return false;

	return logicArray[y][x] == BOX;
}

void Level::setTileAsType(int x, int y, TileType tileType)
{
	if (!isValidPosition(x, y))
		return;

	logicArray[y][x] = tileType;
	sf::Vector2i fillTexture = getTileIndexFromTileType(tileType);
	sf::Vertex* quad = &vertex[(x + y * MAP_WIDTH) * 4];

	// define its 4 corners
	quad[0].position = sf::Vector2f(x * Game::DISPLAY_TILE_SIZE, y * Game::DISPLAY_TILE_SIZE);
	quad[1].position = sf::Vector2f((x + 1) * Game::DISPLAY_TILE_SIZE, y * Game::DISPLAY_TILE_SIZE);
	quad[2].position = sf::Vector2f((x + 1) * Game::DISPLAY_TILE_SIZE, (y + 1) * Game::DISPLAY_TILE_SIZE);
	quad[3].position = sf::Vector2f(x * Game::DISPLAY_TILE_SIZE, (y + 1) * Game::DISPLAY_TILE_SIZE);

	// define its 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(fillTexture.x * Game::TILE_SIZE, fillTexture.y * Game::TILE_SIZE);
	quad[1].texCoords = sf::Vector2f((fillTexture.x + 1) * Game::TILE_SIZE, fillTexture.y * Game::TILE_SIZE);
	quad[2].texCoords = sf::Vector2f((fillTexture.x + 1) * Game::TILE_SIZE, (fillTexture.y + 1) * Game::TILE_SIZE);
	quad[3].texCoords = sf::Vector2f(fillTexture.x * Game::TILE_SIZE, (fillTexture.y + 1) * Game::TILE_SIZE);
}

void Level::setDirtNear(int x, int y, int radius)
{
	setTileAsType(x, y, DIRT);
	sf::Vector2i startPosition = sf::Vector2i(x, y);

	for (auto& direction : directions)
	{
		sf::Vector2i position = startPosition + direction;

		for (int i = 0; i != radius && isValidPosition(position.x, position.y); ++i, position += direction)
		{
			if (isLogicPointCollidable(position))
				break;

			setTileAsType(position.x, position.y, DIRT);
		}
	}
}

void Level::putBomb(int id, sf::Vector2i position, int explosionRadius)
{
	bombs.push_back(Bomb(texture, position.x, position.y, id, explosionRadius));
	std::cout << "[LOG] Bomb with id " << id << " has been put" << std::endl;
}

void Level::update(const sf::Time & deltaTime)
{
	for (auto& bomb : bombs)
		bomb.update(deltaTime);

	for (auto& explosion : explosions)
		explosion->update(deltaTime);

	explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](auto& explosion) { return explosion->canDelete(); }), explosions.end());
}

void Level::explode(int id, std::vector<sf::Vector2i>& destroyedBlocks)
{
	auto iterator = std::find_if(bombs.begin(), bombs.end(), [id](Bomb& bomb) { return bomb.getId() == id; });

	if (iterator == bombs.end())
	{
		std::cout << "[ERROR]: Couldn't explode bomb with id " << id << std::endl;
		return;
	}
	sf::Vector2i bombPosition = getLogicPositionFromRealPosition(iterator->getPosition().x, iterator->getPosition().y);
	explosions.push_back(std::make_unique<Explosion>(texture, bombPosition, iterator->getRadius()));
	setDirtNear(bombPosition.x, bombPosition.y, iterator->getRadius());
	initExplosion(explosions.back().get());

	for (auto& block : destroyedBlocks)
		setTileAsType(block.x, block.y, TileType::DIRT);


	bombs.erase(iterator);
	std::cout << "[LOG]: Deleted bomb with id " << id << std::endl;
}

sf::Vector2f Level::getRealPositionFromLogicPosition(int x, int y)
{
	return sf::Vector2f(x * Game::DISPLAY_TILE_SIZE, y * Game::DISPLAY_TILE_SIZE);
}

sf::Vector2i Level::getLogicPositionFromRealPosition(float x, float y)
{
	return sf::Vector2i(floor(x / static_cast<float>(Game::DISPLAY_TILE_SIZE)), floor(y / static_cast<float>(Game::DISPLAY_TILE_SIZE)));
}


Level::~Level()
{
}
