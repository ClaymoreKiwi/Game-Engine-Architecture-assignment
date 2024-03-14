#pragma once
#include <SDL.h>

class deltaTime
{
public:

	deltaTime() {}

	inline void PartialUpdate()
	{
		currentFrameTime = SDL_GetTicks();
		dTime = static_cast<float>(currentFrameTime - lastFrameTime);
	}

	inline void ManualReset()
	{
		lastFrameTime = currentFrameTime;
	}

	inline float GetDeltaTime() const
	{
		return dTime;
	}
private:
	float  dTime = 0;
	Uint32 currentFrameTime = 0;
	Uint32 lastFrameTime = 0;
};

