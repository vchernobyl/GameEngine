#include "Timing.h"
#include <SDL/SDL.h>

FpsLimiter::FpsLimiter() {
}

void FpsLimiter::Init(float maxFPS) {
    SetMaxFPS(maxFPS);
}

void FpsLimiter::SetMaxFPS(float maxFPS) {
    this->maxFPS = maxFPS;
}

void FpsLimiter::Begin() {
    startTicks = SDL_GetTicks();
}

float FpsLimiter::End() {
    CalculateFPS();
    
    float frameTicks = SDL_GetTicks() - startTicks;
	
    // Limit FPS.
    if (frameTicks < 1000.0f / maxFPS) {
	SDL_Delay(1000.0f / maxFPS - frameTicks);
    }

    return fps;
}

void FpsLimiter::CalculateFPS() {
    static const int numSamples = 10;
    static float frameTimes[numSamples];
    static int currentFrame = 0;

    static float prevTicks = SDL_GetTicks();

    float currentTicks;
    currentTicks = SDL_GetTicks();

    frameTime = currentTicks - prevTicks;
    frameTimes[currentFrame % numSamples] = frameTime;

    prevTicks = currentTicks;

    int count;
    currentFrame++;
    if (currentFrame < numSamples) {
	count = currentFrame;
    } else {
	count = numSamples;
    }

    float frameTimeAverage = 0.0f;
    for (int i = 0; i < count; i++) {
	frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    if (frameTimeAverage > 0) {
	fps = 1000.0f / frameTimeAverage;
    } else {
	fps = 60.0f;
    }
}
