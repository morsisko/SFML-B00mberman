#include "Animation.h"

Animation::Animation(int frameTime, bool looped) : frameTime(frameTime), looped(looped)
{
	;
}

void Animation::update(const sf::Time & time, sf::Sprite& sprite)
{
	if (!looped && isLastFrame())
		return;

	currentFrameTime += time.asMilliseconds();

	if (currentFrameTime < frameTime)
		return;

	currentFrameTime = 0;
	currentFrame++;

	if (currentFrame >= frames.size())
		currentFrame = 0;

	sprite.setTextureRect(frames.at(currentFrame));
}

void Animation::addFrame(sf::IntRect frame)
{
	frames.push_back(frame);
}

bool Animation::isFirstFrame()
{
	return !currentFrame;
}

bool Animation::isLastFrame()
{
	return currentFrame + 1 >= frames.size();
}

void Animation::reset(sf::Sprite& sprite)
{
	currentFrameTime = 0;
	currentFrame = 0;

	if (!frames.empty())
		sprite.setTextureRect(frames.front());
}


Animation::~Animation()
{
}
