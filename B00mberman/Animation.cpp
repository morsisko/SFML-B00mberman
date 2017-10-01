#include "Animation.h"

Animation::Animation(int frameTime) : frameTime(frameTime)
{
	;
}

void Animation::update(const sf::Time & time, sf::Sprite& sprite)
{
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

void Animation::reset()
{
	currentFrameTime = 0;
	currentFrame = 0;
}

Animation::~Animation()
{
}
