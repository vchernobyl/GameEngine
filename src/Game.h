#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Camera.h"
#include "InputManager.h"
#include "Timing.h"
#include <vector>

enum class GameState { Play, Exit };

class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    void InitSystems();
    void InitShaders();
    void RunGameLoop();
    void ProcessInput();
    void DrawGame();

    Window window;
    int screenWidth;
    int screenHeight;
    GameState gameState;

    Shader shader;
    Camera camera;
    InputManager inputManager;
    FpsLimiter fpsLimiter;

    std::vector<class Level*> levels;
    
    float fps;
    float maxFPS;
    int currentLevel;
};
