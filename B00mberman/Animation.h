#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
class Animation
{
private:
	std::vector<sf::IntRect> frames;
	int frameTime = 0;
	int currentFrameTime = 0;
	int currentFrame = 0;
	bool looped = true;
public:
	Animation(int frameTime, bool looped = true);
	void update(const sf::Time& time, sf::Sprite& sprite);
	void addFrame(sf::IntRect frame);
	bool isFirstFrame();
	bool isLastFrame();
	void reset(sf::Sprite& sprite);
	~Animation();
};

