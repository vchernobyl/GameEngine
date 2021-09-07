#pragma once

#include "Sprite.h"
#include "Shader.h"
#include "Texture.h"

enum class GameState { Play, Exit };

class Game {
public:
    Game();
    void Run();

private:
    void InitSystems();
    void InitShaders();
    void RunGameLoop();
    void ProcessInput();
    void DrawGame();

    class SDL_Window* window;
    int screenWidth;
    int screenHeight;
    GameState gameState;
    Sprite sprite;
    Shader shader;
    Texture playerTexture;

    float time;
};
