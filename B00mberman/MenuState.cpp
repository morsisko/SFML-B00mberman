#include "MenuState.h"




MenuState::MenuState(GameStateManager* manager, sf::RenderWindow* window) : State(manager, window),
	playBtn(std::bind(&MenuState::play, this))
{
	font.loadFromFile("assets/ariblk.ttf");

	playBtn.setFont(font);
	playBtn.setString("Play");
	playBtn.setCharacterSize(50);
	playBtn.setPosition(window->getSize().x / 2 - playBtn.getGlobalBounds().width / 2, window->getSize().y / 2 - playBtn.getGlobalBounds().height / 2);
}

void MenuState::handleEvent(const sf::Event & event)
{
	playBtn.handleEvent(event);
}

void MenuState::render()
{
	window->draw(playBtn);
}

void MenuState::update(const sf::Time & deltaTime)
{
}

void MenuState::play()
{
	manager->setState(std::make_unique<GameState>(manager, window));
}

MenuState::~MenuState()
{
}
