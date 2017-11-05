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

	else if (header == PUT_BOMB)
		handlePutBomb(packet);

	else if (header == EXPLODE)
		handleExplosion(packet);

	else if (header == TP)
		handleTp(packet);

	else
		std::cout << "[ERROR]: Unknown packet with header " << headerNumber << " has been recv" << std::endl;
}

void GameState::handleEnemyMove(sf::Packet & packet)
{
	sf::Uint8 x;
	sf::Uint8 y;

	packet >> x >> y;

	netPlayer.move(sf::Vector2i(x, y));
}

void GameState::handlePutBomb(sf::Packet & packet)
{
	sf::Uint32 id;
	sf::Uint8 x;
	sf::Uint8 y;
	sf::Uint8 explosionRadius;

	packet >> id >> x >> y >> explosionRadius;

	level.putBomb(id, sf::Vector2i(x, y), explosionRadius);
}

void GameState::handleExplosion(sf::Packet & packet)
{
	sf::Uint32 id;
	sf::Uint8 destroyedBlocks;
	sf::Uint8 isGameEnd;
	sf::Uint8 gameEndState;

	packet >> id >> destroyedBlocks;

	std::vector<sf::Vector2i> positions;

	for (int i = 0; i != destroyedBlocks; ++i)
	{
		sf::Uint8 x;
		sf::Uint8 y;
		packet >> x >> y;
		positions.push_back(sf::Vector2i(x, y));
	}

	packet >> isGameEnd;

	if (isGameEnd)
	{
		packet >> gameEndState;
		endGame(static_cast<PlayerGameState>(gameEndState));
	}

	level.explode(id, positions);
}

void GameState::handleTp(sf::Packet & packet)
{
	sf::Uint8 x;
	sf::Uint8 y;

	packet >> x >> y;

	localPlayer.tp(x, y);
	std::cout << "[LOG]: Player has been tp" << std::endl;
}

void GameState::endGame(PlayerGameState gameEndState)
{
	gameEnd = true;
	std::string string;

	if (gameEndState == DRAW)
		string = "It's DRAW!";

	else if (gameEndState == WIN)
		string = "You WON!";

	else
		string = "You LOST!";

	string += "\nPress ENTER\nto go to menu";
	endText.setString(string);

	endText.setPosition(window->getSize().x / 2.0f - endText.getGlobalBounds().width / 2.0f, window->getSize().y / 2.0f - endText.getGlobalBounds().height / 2.0f);
}

void GameState::gameEndEventHandle(const sf::Event & event)
{
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
		manager->setState(std::make_unique<MenuState>(manager, window));

}

GameState::GameState(GameStateManager* manager, sf::RenderWindow* window, std::unique_ptr<sf::TcpSocket> server, std::array<std::array<int, Level::MAP_WIDTH>, Level::MAP_HEIGHT> &logicArray,
	sf::Vector2i localPlayerPosition, PlayerAppearance localPlayerAppearance, 
	sf::Vector2i netPlayerPosition, PlayerAppearance netPlayerAppearance) : State(manager, window),
	level(manager->getAssets().getTileTexture(), logicArray),
	networkManager(std::move(server)),
	localPlayer(manager->getAssets().getTileTexture(), level, networkManager, localPlayerPosition, localPlayerAppearance),
	netPlayer(manager->getAssets().getTileTexture(), level, netPlayerPosition, netPlayerAppearance)
{
	endText.setCharacterSize(50);
	endText.setOutlineThickness(5);
	endText.setFont(manager->getAssets().getFont());
}

void GameState::handleEvent(const sf::Event & event)
{
	if (!gameEnd)
		localPlayer.handleEvent(event);
	else
		gameEndEventHandle(event);
}

void GameState::render()
{
	window->draw(level);
	window->draw(localPlayer);
	window->draw(netPlayer);

	if (gameEnd)
		window->draw(endText);
}

void GameState::update(const sf::Time & deltaTime)
{
	if (!gameEnd)
	{
		localPlayer.update(deltaTime);
		netPlayer.update(deltaTime);
		level.update(deltaTime);
	}
	networkManager.process();
	handlePackets();
}


GameState::~GameState()
{
}
