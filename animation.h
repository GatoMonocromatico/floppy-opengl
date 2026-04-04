#pragma once

#include "Timer.h"

class Animation {
	Timer timer;
	int frameCount, currentFrame;
	float spriteUVOffsetX, spriteUVOffsetY;
	int framesPerRow;

public:
	Animation() : timer(0), frameCount(0) {}
	Animation(float length, int frameCount, int spriteWidth, int spriteHeight, int spriteSheetWidth, int spriteSheetHeight, int currentFrame=0) :
		timer(length),
		frameCount(frameCount),
		currentFrame(currentFrame)
	{
		spriteUVOffsetX = spriteWidth / (float) spriteSheetWidth;
		spriteUVOffsetY = spriteHeight / (float) spriteSheetHeight;

		framesPerRow = spriteSheetWidth / spriteWidth;

	}
	float getLength() const { return timer.getLength(); }

	int getCurrentFrame() const
	{
		return currentFrame;
	}

	float getSpriteUVOffsetX() const
	{
		return static_cast<int>(getCurrentFrame() % framesPerRow) * spriteUVOffsetX;
	}
	float getSpriteUVOffsetY() const
	{
		return -static_cast<int>(getCurrentFrame() / framesPerRow) * spriteUVOffsetY;
	}


	void step(float deltaTime)
	{
		timer.step(deltaTime);
		if (timer.isTimedOut() && frameCount > currentFrame + 1)
		{
			currentFrame = (currentFrame + 1) % frameCount;
			timer.restart();
		}
	}
	float getTime() const
	{
		return timer.getTime();
	}
};