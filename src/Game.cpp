#include "Game.h"
#include "Engine.h"
#include "Errors.h"
#include "ResourceManager.h"
#include "Level.h"
#include "Zombie.h"
#include "Human.h"
#include <random>
#include <ctime>

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

Game::Game() :
    screenWidth(1024),
    screenHeight(768),
    gameState(GameState::Play),
    fps(0.0f),
    maxFPS(60.0f),
    player(nullptr) {
}

Game::~Game() {
    for (auto level : levels) {
	delete level;
    }
}

void Game::Run() {
    InitSystems();
    InitLevel();
    RunGameLoop();
}

void Game::InitSystems() {
    Engine::Init();
    
    window.Create("Game Engine", screenWidth, screenHeight, 0);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    InitShaders();
    agentSpriteBatch.Init();
    camera.Init(screenWidth, screenHeight);
    fpsLimiter.Init(maxFPS);
}

void Game::InitLevel() {
    levels.push_back(new Level("data/level_1.txt"));
    currentLevel = 0;

    player = new Player();
    player->Init(5.0f, levels[currentLevel]->GetPlayerStartPosition(), &inputManager);
    humans.push_back(player);

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));
    std::uniform_int_distribution<int> randX(2, levels[currentLevel]->GetWidth() - 1);
    std::uniform_int_distribution<int> randY(2, levels[currentLevel]->GetHeight() - 1);

    for (int i = 0; i < levels[currentLevel]->GetNumHumans(); i++) {
	humans.push_back(new Human);
	glm::vec2 pos(randX(randomEngine) * TileSize, randY(randomEngine) * TileSize);
	humans.back()->Init(1.0f, pos);
    }
}

void Game::InitShaders() {
    shader.Compile("data/shaders/TextureShading.vert", "data/shaders/TextureShading.frag");
    shader.AddAttribute("vertexPosition");
    shader.AddAttribute("vertexColor");
    shader.AddAttribute("vertexUV");
    shader.Link();
}

void Game::UpdateAgents() {
    for (auto agent : humans) {
	agent->Update(levels[currentLevel]->GetLevelData(), humans, zombies);
    }

    for (int i = 0; i < humans.size(); i++) {
	for (int j = i + 1; j < humans.size(); j++) {
	    humans[i]->CollideWithAgent(humans[j]);
	}
    }
}

void Game::RunGameLoop() {
    while (gameState != GameState::Exit) {
	fpsLimiter.Begin();

	ProcessInput();
	UpdateAgents();
	
	camera.SetPosition(player->GetPosition());
	camera.Update();
	
	DrawGame();

	fps = fpsLimiter.End();
    }	
}

void Game::ProcessInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	    gameState = GameState::Exit;
	    break;
	case SDL_KEYDOWN:
	    inputManager.PressKey(event.key.keysym.sym);
	    break;
	case SDL_KEYUP:
	    inputManager.ReleaseKey(event.key.keysym.sym);
	    break;
	case SDL_MOUSEBUTTONDOWN:
	    inputManager.PressKey(event.button.button);
	    break;
	case SDL_MOUSEBUTTONUP:
	    inputManager.ReleaseKey(event.button.button);
	    break;
	case SDL_MOUSEMOTION:
	    inputManager.SetMouseCoords(event.motion.x, event.motion.y);
	    break;
	}
    }
    
    // const float cameraSpeed = 5.0f;
    // const float scaleSpeed = 0.05f;

    // if (inputManager.IsKeyPressed(SDLK_w)) {
    // 	camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, cameraSpeed));
    // }
    // if (inputManager.IsKeyPressed(SDLK_s)) {
    // 	camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, -cameraSpeed));
    // }
    // if (inputManager.IsKeyPressed(SDLK_a)) {
    // 	camera.SetPosition(camera.GetPosition() + glm::vec2(-cameraSpeed, 0.0f));
    // }
    // if (inputManager.IsKeyPressed(SDLK_d)) {
    // 	camera.SetPosition(camera.GetPosition() + glm::vec2(cameraSpeed, 0.0f));
    // }
    // if (inputManager.IsKeyPressed(SDLK_q)) {
    // 	camera.SetScale(camera.GetScale() + scaleSpeed);
    // }
    // if (inputManager.IsKeyPressed(SDLK_e)) {
    // 	camera.SetScale(camera.GetScale() - scaleSpeed);
    // }

    // if (inputManager.IsKeyPressed(SDL_BUTTON_LEFT)) {
    // 	glm::vec2 mouseCoords = inputManager.GetMouseCoords();
    // 	mouseCoords = camera.ScreenToWorld(mouseCoords);
    // 	glm::vec2 playerPosition(0.0f);
    // 	glm::vec2 direction = mouseCoords - playerPosition;
    // 	direction = glm::normalize(direction);
    // 	bullets.emplace_back(playerPosition, direction, 10.0f, 120);
    // }
}

void Game::DrawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    // Looks like these 3 lines are optional.
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = shader.GetUniformLocation("spriteTexture");
    glUniform1i(textureUniform, 0);

    glm::mat4 projectionMatrix = camera.GetCameraMatrix();
    GLuint pUniform = shader.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    levels[currentLevel]->Draw();

    agentSpriteBatch.Begin();
    for (auto human : humans) {
	human->Draw(agentSpriteBatch);
    }
    agentSpriteBatch.End();
    agentSpriteBatch.DrawBatch();

    shader.Unuse();

    window.SwapBuffer();
}
