#include "Player.h"



Player::Player()
{
}

void Player::sendPacket(sf::Packet & packet)
{
	packetsQueue.push_back(packet);
}

void Player::setAppearance(PlayerAppearance appearance)
{
	this->appearance = appearance;
}

void Player::setPosition(sf::Vector2i position)
{
	this->position = position;
}

void Player::endLife()
{
	kill = true;
}

const sf::Vector2i & Player::getPosition()
{
	return position;
}

int Player::getBombExplosionRadius()
{
	return bombExplosionRadius;
}

int Player::getMaxBombs()
{
	return maxBombs;
}

bool Player::isKilled()
{
	return kill;
}

void Player::processPackets()
{
	if (packetsQueue.empty())
		return;

	if (send(packetsQueue.front()) == sf::Socket::Status::Done)
		packetsQueue.pop_front();

}

void Player::sendInitPacket(std::array<std::array<int, NetGame::MAP_WIDTH>, NetGame::MAP_HEIGHT>& logicArray, Player* opponent)
{
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(ServerPackets::INIT_GAME);

	for (auto& tileSet : logicArray)
		for (auto& tile : tileSet)
			packet << static_cast<sf::Uint8>(tile);

	packet << static_cast<sf::Uint8>(position.x) << static_cast<sf::Uint8>(position.y) << static_cast<sf::Uint8>(appearance)
			<< static_cast<sf::Uint8>(opponent->position.x) << static_cast<sf::Uint8>(opponent->position.y) << static_cast<sf::Uint8>(opponent->appearance);

	this->sendPacket(packet);

}

void Player::sendOpponentMove(Player * opponent)
{
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(ServerPackets::MOVE_ENEMY) << static_cast<sf::Uint8>(opponent->getPosition().x) << static_cast<sf::Uint8>(opponent->getPosition().y);

	this->sendPacket(packet);
}

void Player::sendBombInfo(ServerBomb & bomb)
{
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(ServerPackets::PUT_BOMB) << static_cast<sf::Uint32>(bomb.getId()) << static_cast<sf::Uint8>(bomb.getPosition().x) << static_cast<sf::Uint8>(bomb.getPosition().y)
			<< static_cast<sf::Uint8>(bomb.getExplosionRadius());

	this->sendPacket(packet);
}

void Player::sendExplosionInfo(ServerBomb& bomb, std::vector<sf::Vector2i>& destroyedBlocks, bool gameEnd, bool draw)
{
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(ServerPackets::EXPLODE) << static_cast<sf::Uint32>(bomb.getId()) << static_cast<sf::Uint8>(destroyedBlocks.size());

	for (auto& block : destroyedBlocks)
		packet << static_cast<sf::Uint8>(block.x) << static_cast<sf::Uint8>(block.y);

	packet << static_cast<sf::Uint8>(gameEnd);

	if (gameEnd)
	{
		if (draw)
			packet << static_cast<sf::Uint8>(PlayerGameState::DRAW);

		else if (kill)
			packet << static_cast<sf::Uint8>(PlayerGameState::LOSE);

		else
			packet << static_cast<sf::Uint8>(PlayerGameState::WIN);
	}


	this->sendPacket(packet);
}

void Player::sendTp(sf::Vector2i position)
{
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(ServerPackets::TP) << static_cast<sf::Uint8>(position.x) << static_cast<sf::Uint8>(position.y);

	this->sendPacket(packet);
}


std::string Player::getName()
{
	if (appearance == GREEN_ORC)
		return "Green Orc";

	return "Blue Orc";
}

Player::~Player()
{
}
