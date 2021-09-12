#include "Game.h"
#include "Engine.h"
#include "Errors.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

Game::Game() :
    screenWidth(1024),
    screenHeight(768),
    gameState(GameState::Play),
    time(0.0f),
    fps(0.0f),
    maxFPS(60.0f),
    frameTime(0.0f) {
    camera.Init(screenWidth, screenHeight);
}

void Game::Run() {
    InitSystems();

    sprites.push_back(new Sprite());
    sprites.back()->Init(0.0f, 0.0f, screenWidth / 2, screenWidth / 2,
			 "data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

    sprites.push_back(new Sprite());
    sprites.back()->Init(screenWidth / 2, 0.0f, screenWidth / 2, screenWidth / 2,
			 "data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

    RunGameLoop();
}

void Game::InitSystems() {
    Engine::Init();
    
    // TODO: Init window somewhere here...
    window.Create("Game Engine", screenWidth, screenHeight, 0);

    InitShaders();
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
	float startTicks = SDL_GetTicks();

	ProcessInput();

	// Temporary stuff
	time += 0.01f;
	camera.Update();

	DrawGame();
	CalculateFPS();

	static int frameCounter = 0;
	if (frameCounter++ == 10) {
	    std::cout << fps << std::endl;
	    frameCounter = 0;
	}

	float frameTicks = SDL_GetTicks() - startTicks;
	
	// Limit FPS.
	if (frameTicks < 1000.0f / maxFPS) {
	    SDL_Delay(1000.0f / maxFPS - frameTicks);
	}
    }	
}

void Game::ProcessInput() {
    SDL_Event event;

    const float cameraSpeed = 10.0f;
    const float scaleSpeed = 0.1f;

    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	    gameState = GameState::Exit;
	    break;
	case SDL_KEYDOWN:
	    switch (event.key.keysym.sym) {
	    case SDLK_w:
		camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, cameraSpeed));
		break;
	    case SDLK_s:
		camera.SetPosition(camera.GetPosition() + glm::vec2(0.0f, -cameraSpeed));
		break;
	    case SDLK_a:
		camera.SetPosition(camera.GetPosition() + glm::vec2(-cameraSpeed, 0.0f));
		break;
	    case SDLK_d:
		camera.SetPosition(camera.GetPosition() + glm::vec2(cameraSpeed, 0.0f));
		break;
	    case SDLK_q:
		camera.SetScale(camera.GetScale() + scaleSpeed);
		break;
	    case SDLK_e:
		camera.SetScale(camera.GetScale() - scaleSpeed);
		break;
	    }
	    break;
	}
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

    for (auto sprite : sprites) {
	sprite->Draw();
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    shader.Unuse();

    window.SwapBuffer();
}

void Game::CalculateFPS() {
    static const int numSamples = 10;
    static float frameTimes[numSamples];
    static int currentFrame = 0;

    static float prevTicks = SDL_GetTicks();

    float currentTicks;
    currentTicks = SDL_GetTicks();

    frameTime = currentTicks - prevTicks;
    frameTimes[currentFrame % numSamples] = frameTime;

    prevTicks = currentTicks;

    int count;
    currentFrame++;
    if (currentFrame < numSamples) {
	count = currentFrame;
    } else {
	count = numSamples;
    }

    float frameTimeAverage = 0.0f;
    for (int i = 0; i < count; i++) {
	frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    if (frameTimeAverage > 0) {
	fps = 1000.0f / frameTimeAverage;
    } else {
	fps = 60.0f;
    }
}
