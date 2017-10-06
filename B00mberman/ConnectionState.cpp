#include "ConnectionState.h"
#include "MenuState.h"


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
	if (!shouldTryToConnect)
		return;

	currentElapsedConnectionTime += deltaTime.asSeconds();

	if (currentElapsedConnectionTime >= MAX_CONNECTION_SECONDS)
	{
		shouldTryToConnect = false;
		text.setString("Failed to connect.\nBack to menu\nand try again!");
		centerText();
		return;
	}

	sf::Socket::Status status = socket->connect(sf::IpAddress::LocalHost, 1337);

	if (status == sf::Socket::Done)
	{
		shouldTryToConnect = false;
		text.setString("Connected!");
		centerText();
	}
}

ConnectionState::~ConnectionState()
{
}
