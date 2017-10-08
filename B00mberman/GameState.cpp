#include "GameState.h"

void GameState::handlePackets()
{
	sf::Packet packet;
	if (!networkManager.recvPacket(packet))
		return;

	sf::Uint8 headerNumber;
	packet >> headerNumber;

	ServerPackets header = static_cast<ServerPackets>(headerNumber);

	if (header == MOVE_ENEMY)
		handleEnemyMove(packet);
	else
		std::cout << "[ERROR]: Unknown packet with header " << headerNumber << " has been recv" << std::endl;
}

void GameState::handleEnemyMove(sf::Packet & packet)
{
	sf::Uint8 x;
	sf::Uint8 y;

	packet >> x >> y;

	netPlayer.move(sf::Vector2i(x, y));
	std::cout << "[INFO]: Enemy move registered" << std::endl;
}

GameState::GameState(GameStateManager* manager, sf::RenderWindow* window, std::unique_ptr<sf::TcpSocket> server, std::array<std::array<int, Level::MAP_WIDTH>, Level::MAP_HEIGHT> &logicArray,
	sf::Vector2i localPlayerPosition, PlayerAppearance localPlayerAppearance, 
	sf::Vector2i netPlayerPosition, PlayerAppearance netPlayerAppearance) : State(manager, window),
	level(manager->getAssets().getTileTexture(), logicArray),
	networkManager(std::move(server)),
	localPlayer(manager->getAssets().getTileTexture(), level, networkManager, localPlayerPosition, localPlayerAppearance),
	netPlayer(manager->getAssets().getTileTexture(), level, netPlayerPosition, netPlayerAppearance)
{
	;
}

void GameState::handleEvent(const sf::Event & event)
{
	localPlayer.handleEvent(event);
}

void GameState::render()
{
	window->draw(level);
	window->draw(localPlayer);
	window->draw(netPlayer);
}

void GameState::update(const sf::Time & deltaTime)
{
	localPlayer.update(deltaTime);
	netPlayer.update(deltaTime);
	level.update(deltaTime);
	networkManager.process();
	handlePackets();
}


GameState::~GameState()
{
}
