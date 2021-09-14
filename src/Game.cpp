#include "Game.h"
#include "Engine.h"
#include "Errors.h"
#include "ResourceManager.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

Game::Game() :
    screenWidth(1024),
    screenHeight(768),
    gameState(GameState::Play),
    time(0.0f),
    fps(0.0f),
    maxFPS(60.0f) {
    camera.Init(screenWidth, screenHeight);
}

void Game::Run() {
    InitSystems();
    RunGameLoop();
}

void Game::InitSystems() {
    Engine::Init();
    
    // TODO: Init window somewhere here...
    window.Create("Game Engine", screenWidth, screenHeight, 0);

    InitShaders();
    spriteBatch.Init();
    fpsLimiter.Init(maxFPS);
}

void Game::InitShaders() {
    shader.Compile("data/shaders/ColorShading.vert", "data/shaders/ColorShading.frag");
    shader.AddAttribute("vertexPosition");
    shader.AddAttribute("vertexColor");
    shader.AddAttribute("vertexUV");
    shader.Link();
}

void Game::RunGameLoop() {
    while (gameState != GameState::Exit) {
	fpsLimiter.Begin();

	ProcessInput();

	// Temporary stuff
	time += 0.01f;
	camera.Update();

	DrawGame();

	fps = fpsLimiter.End();

	static int frameCounter = 0;
	if (frameCounter++ == 10) {
	    std::cout << fps << std::endl;
	    frameCounter = 0;
	}
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
}

void Game::DrawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    
    GLint textureLocation = shader.GetUniformLocation("spriteTexture");
    glUniform1i(textureLocation, 0);

    GLuint timeLocation = shader.GetUniformLocation("time");
    glUniform1f(timeLocation, time);

    GLuint pLocation = shader.GetUniformLocation("P");
    glm::mat4 cameraMatrix = camera.GetCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

    spriteBatch.Begin();

    glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Texture texture = ResourceManager::GetTexture("data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
    static Texture texture2 = ResourceManager::GetTexture("data/textures/jimmyJump_pack/PNG/CharacterLeft_Standing.png");
    Color color;
    color.r = 255.0f;
    color.g = 255.0f;
    color.b = 255.0f;
    color.a = 255.0f;

    spriteBatch.Draw(pos, uv, texture.id, 0.0f, color);
    spriteBatch.Draw(pos + glm::vec4(50.0f, 0.0f, 0.0f, 0.0f), uv, texture2.id, 0.0f, color);

    spriteBatch.End();
    spriteBatch.DrawBatch();

    glBindTexture(GL_TEXTURE_2D, 0);
    shader.Unuse();

    window.SwapBuffer();
}
