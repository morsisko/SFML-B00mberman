#pragma once
#include "State.h"
#include "SimpleButton.h"
#include <SFML\Network.hpp>

class ConnectionState :
	public State
{
private:
	static constexpr float MAX_CONNECTION_SECONDS = 10.0f;

	sf::Text text;
	SimpleButton button;
	float currentElapsedConnectionTime = 0.0f;
	std::unique_ptr<sf::TcpSocket> socket;
	bool shouldTryToConnect = false;

	void onMenuButtonClicked();
	void initConnection();
	void centerText();
public:
	ConnectionState(GameStateManager* manager, sf::RenderWindow* window, std::unique_ptr<sf::TcpSocket> socket = nullptr);
	virtual void handleEvent(const sf::Event &event) override;
	virtual void render() override;
	virtual void update(const sf::Time &deltaTime) override;
	virtual ~ConnectionState();
};
