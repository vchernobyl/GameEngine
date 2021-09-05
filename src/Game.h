#pragma once

#include "Sprite.h"

enum class GameState { Play, Exit };

class Game {
public:
    Game();
    void Run();

private:
    void InitSystems();
    void RunGameLoop();
    void ProcessInput();
    void DrawGame();

    class SDL_Window* window;
    int screenWidth;
    int screenHeight;
    GameState gameState;
    Sprite sprite;
};
