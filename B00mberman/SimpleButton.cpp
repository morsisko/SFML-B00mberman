#include "SimpleButton.h"



SimpleButton::SimpleButton(std::function<void()> onClick, sf::Color defaultColor, sf::Color markColor) : onClick(onClick), defaultColor(defaultColor), markColor(markColor)
{
	sf::Text::setFillColor(defaultColor);
}

void SimpleButton::handleEvent(const sf::Event & event)
{
	switch (event.type)
	{
		case sf::Event::MouseMoved:
		{
			bool contains = getGlobalBounds().contains(sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)));
			if (contains && !focused)
			{
				focused = true;
				sf::Text::setFillColor(markColor);
			}
			else if (!contains && focused)
			{
				focused = false;
				sf::Text::setFillColor(defaultColor);
			}
		}

		case sf::Event::MouseButtonReleased:
		{
			bool contains = getGlobalBounds().contains(sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)));

			if (contains)
				onClick();
		}
	}
}


SimpleButton::~SimpleButton()
{
}
