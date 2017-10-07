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

const sf::Vector2i & Player::getPosition(sf::Vector2i position)
{
	return position;
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


Player::~Player()
{
}
