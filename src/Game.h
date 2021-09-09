#pragma once

#include "Sprite.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>

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
    void CalculateFPS();

    class SDL_Window* window;
    int screenWidth;
    int screenHeight;
    GameState gameState;
    std::vector<Sprite*> sprites;
    Shader shader;

    float time;

    float fps;
    float maxFPS;
    float frameTime;
};
