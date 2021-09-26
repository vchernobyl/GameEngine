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
#include "AudioManager.h"
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
    void UpdateAgents(float deltaTime);
    void RunGameLoop();
    void ProcessInput();
    void DrawGame();

    Window window;
    int screenWidth = 1024;
    int screenHeight = 768;
    GameState gameState = GameState::Play;

    Shader shader;
    Camera camera;
    SpriteBatch agentSpriteBatch;
    InputManager inputManager;
    AudioManager audioManager;
    FpsLimiter fpsLimiter;

    std::vector<class Level*> levels;
    std::vector<class Human*> humans;
    std::vector<class Zombie*> zombies;
    
    Player* player = nullptr;
    
    float fps = 0.0f;
    float maxFPS = 60.0f;
    int currentLevel;
};
