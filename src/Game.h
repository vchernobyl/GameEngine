#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Camera.h"
#include "InputManager.h"
#include "Timing.h"
#include "Human.h"
#include "Agent.h"
#include "Player.h"
#include "SpriteBatch.h"
#include <vector>

enum class GameState { Play, Exit };

class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    void InitSystems();
    void InitLevel();
    void InitShaders();
    void UpdateAgents();
    void RunGameLoop();
    void ProcessInput();
    void DrawGame();

    Window window;
    int screenWidth;
    int screenHeight;
    GameState gameState;

    Shader shader;
    Camera camera;
    SpriteBatch agentSpriteBatch;
    InputManager inputManager;
    FpsLimiter fpsLimiter;

    std::vector<class Level*> levels;
    std::vector<class Human*> humans;
    std::vector<class Zombie*> zombies;
    
    Player* player;
    
    float fps;
    float maxFPS;
    int currentLevel;
};
