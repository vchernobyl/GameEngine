#include "Game.h"
#include "Errors.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

Game::Game() :
    window(nullptr),
    screenWidth(1024),
    screenHeight(768),
    gameState(GameState::Play),
    time(0.0f),
    fps(0.0f),
    maxFPS(60.0f),
    frameTime(0.0f) {
}

void Game::Run() {
    InitSystems();

    sprites.push_back(new Sprite());
    sprites.back()->Init(-1.0f, -1.0f, 1.0f, 1.0f, "data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

    sprites.push_back(new Sprite());
    sprites.back()->Init(0.0f, -1.0f, 1.0f, 1.0f, "data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

    RunGameLoop();
}

void Game::InitSystems() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			      screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
	FatalError("SDL_Window could not be created.");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
	FatalError("SDL_GLContext could not be created.");
    }

    GLenum error = glewInit();
    if (error != GLEW_OK) {
	FatalError("Could not initialize glew.");
    }

    std::printf("*** OpenGL version: %s ***\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    SDL_GL_SetSwapInterval(0);

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
	time += 0.01f;
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
    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	    gameState = GameState::Exit;
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

    for (auto sprite : sprites) {
	sprite->Draw();
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    shader.Unuse();

    SDL_GL_SwapWindow(window);
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
