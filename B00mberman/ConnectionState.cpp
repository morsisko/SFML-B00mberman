#include "ConnectionState.h"
#include "MenuState.h"
#include "GameState.h"
#include "LocalPlayer.h"


void ConnectionState::onMenuButtonClicked()
{
	manager->setState(std::make_unique<MenuState>(manager, window));
}

void ConnectionState::initConnection()
{
	socket = std::make_unique<sf::TcpSocket>();
	socket->setBlocking(false);
	shouldTryToConnect = true;
}

void ConnectionState::centerText()
{
	text.setPosition(window->getSize().x / 2 - text.getGlobalBounds().width / 2, window->getSize().y / 2 - text.getGlobalBounds().height / 2);
}

void ConnectionState::tryToRecvPackets()
{
	sf::Packet packet;
	sf::Socket::Status status = socket->receive(packet);

	if (status == sf::Socket::Done)
		handlePacket(packet);

	else if (status == sf::Socket::Disconnected)
	{
		std::cout << "[ERROR]: Socket disconnected" << std::endl;
		connected = false;
	}
}

void ConnectionState::tryToConnect(const sf::Time& deltaTime)
{
	currentElapsedConnectionTime += deltaTime.asSeconds();

	if (currentElapsedConnectionTime >= MAX_CONNECTION_SECONDS)
	{
		shouldTryToConnect = false;
		text.setString("Failed to connect.\nBack to menu\nand try again!");
		centerText();
	}

	else
	{
		sf::Socket::Status status = socket->connect("192.168.1.100", 25565);

		if (status == sf::Socket::Done)
		{
			connected = true;
			shouldTryToConnect = false;
			text.setString("Connected!");
			centerText();
		}
	}
}

void ConnectionState::handlePacket(sf::Packet & packet)
{
	sf::Uint8 headerNumber;
	packet >> headerNumber;

	ServerPackets header = static_cast<ServerPackets>(headerNumber);

	if (header == ServerPackets::INIT_GAME)
		parseInitGamePacket(packet);

	else
		std::cout << "[UNKNOWN HEADER]: " << headerNumber << std::endl;


}

void ConnectionState::parseInitGamePacket(sf::Packet & packet)
{
	std::array<std::array<int, Level::MAP_WIDTH>, Level::MAP_HEIGHT> logicArray;

	for (auto& tileSet : logicArray)
	{
		for (auto& tile : tileSet)
		{
			sf::Uint8 recvTile;
			packet >> recvTile;

			tile = recvTile;
		}
	}

	sf::Uint8 localPlayerX;
	sf::Uint8 localPlayerY;
	sf::Uint8 localPlayerAppearance;

	sf::Uint8 netPlayerX;
	sf::Uint8 netPlayerY;
	sf::Uint8 netPlayerAppearance;

	packet >> localPlayerX >> localPlayerY >> localPlayerAppearance >> netPlayerX >> netPlayerY >> netPlayerAppearance;

	manager->setState(std::make_unique<GameState>(manager, window, std::move(socket), logicArray, sf::Vector2i(localPlayerX, localPlayerY), static_cast<PlayerAppearance>(localPlayerAppearance), sf::Vector2i(netPlayerX, netPlayerY), static_cast<PlayerAppearance>(netPlayerAppearance)));
}

ConnectionState::ConnectionState(GameStateManager * manager, sf::RenderWindow * window, std::unique_ptr<sf::TcpSocket> socket) : State(manager, window),
	button(std::bind(&ConnectionState::onMenuButtonClicked, this))
{
	text.setFont(manager->getAssets().getFont());
	text.setCharacterSize(60);
	text.setString("Connecting");
	centerText();

	button.setFont(manager->getAssets().getFont());
	button.setCharacterSize(60);
	button.setString("Back to menu");
	button.setPosition(window->getSize().x / 2 - button.getGlobalBounds().width / 2, window->getSize().y / 1.3f - button.getGlobalBounds().height / 2);

	if (!socket)
		initConnection();
	else
		this->socket = std::move(socket);

}

void ConnectionState::handleEvent(const sf::Event & event)
{
	button.handleEvent(event);
}

void ConnectionState::render()
{
	window->draw(text);
	window->draw(button);
}

void ConnectionState::update(const sf::Time & deltaTime)
{
	if (connected)
		tryToRecvPackets();

	else if (shouldTryToConnect)
		tryToConnect(deltaTime);


}

ConnectionState::~ConnectionState()
{
}
