#pragma once

class FpsLimiter {
public:
    FpsLimiter();

    void Init(float maxFPS);
    void SetMaxFPS(float maxFPS);
    void Begin();
    float End();

private:
    void CalculateFPS();
    
    float fps;
    float maxFPS;
    float frameTime;
    unsigned int startTicks;
};
