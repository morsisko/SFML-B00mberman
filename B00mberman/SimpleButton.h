#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
class SimpleButton : public sf::Text
{
private:
	std::function<void()> onClick;
	sf::Color defaultColor;
	sf::Color markColor;
	bool focused = false;
public:
	SimpleButton(std::function<void()> onClick, sf::Color defaultColor = sf::Color::White, sf::Color markColor = sf::Color::Blue);
	void handleEvent(const sf::Event &event);
	~SimpleButton();
};

