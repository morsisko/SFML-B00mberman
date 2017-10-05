#include "Player.h"



Player::Player()
{
}

void Player::sendPacket(sf::Packet & packet)
{
	packetsQueue.push_back(packet);
}

void Player::processPackets()
{
	if (packetsQueue.empty())
		return;

	if (send(packetsQueue.front()) == sf::Socket::Status::Done)
		packetsQueue.pop_front();

}


Player::~Player()
{
}
