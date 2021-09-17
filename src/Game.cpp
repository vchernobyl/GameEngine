#include "Game.h"
#include "Engine.h"
#include "Errors.h"
#include "ResourceManager.h"
#include "Level.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

Game::Game() :
    screenWidth(1024),
    screenHeight(768),
    gameState(GameState::Play),
    fps(0.0f),
    maxFPS(60.0f),
    currentLevel(0) {
}

Game::~Game() {
    for (auto level : levels) {
	delete level;
    }
}

void Game::Run() {
    InitSystems();
    RunGameLoop();
}

void Game::InitSystems() {
    Engine::Init();
    
    // TODO: Init window somewhere here...
    window.Create("Game Engine", screenWidth, screenHeight, 0);

    levels.push_back(new Level("data/level_1.txt"));

    InitShaders();
    camera.Init(screenWidth, screenHeight);
    fpsLimiter.Init(maxFPS);
}

void Game::InitShaders() {
    shader.Compile("data/shaders/TextureShading.vert", "data/shaders/TextureShading.frag");
    shader.AddAttribute("vertexPosition");
    shader.AddAttribute("vertexColor");
    shader.AddAttribute("vertexUV");
    shader.Link();
}

void Game::RunGameLoop() {
    while (gameState != GameState::Exit) {
	fpsLimiter.Begin();

	ProcessInput();

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
    
    const float cameraSpeed = 5.0f;
    const float scaleSpeed = 0.05f;

    if (inputManager.IsKeyPressed(SDLK_w)) {
	camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, cameraSpeed));
    }
    if (inputManager.IsKeyPressed(SDLK_s)) {
	camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, -cameraSpeed));
    }
    if (inputManager.IsKeyPressed(SDLK_a)) {
	camera.SetPosition(camera.GetPosition() + glm::vec2(-cameraSpeed, 0.0f));
    }
    if (inputManager.IsKeyPressed(SDLK_d)) {
	camera.SetPosition(camera.GetPosition() + glm::vec2(cameraSpeed, 0.0f));
    }
    if (inputManager.IsKeyPressed(SDLK_q)) {
	camera.SetScale(camera.GetScale() + scaleSpeed);
    }
    if (inputManager.IsKeyPressed(SDLK_e)) {
	camera.SetScale(camera.GetScale() - scaleSpeed);
    }

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

    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = shader.GetUniformLocation("spriteTexture");
    glUniform1i(textureUniform, 0);

    glm::mat4 projectionMatrix = camera.GetCameraMatrix();
    GLuint pUniform = shader.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    levels[currentLevel]->Draw();

    shader.Unuse();

    window.SwapBuffer();
}
