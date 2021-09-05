#include "Game.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

void FatalError(const std::string& error) {
    std::cout << error << std::endl;
    std::cout << "Enter any key to quit...";
    int temp;
    std::cin >> temp;
    SDL_Quit();
}

Game::Game() {
    window = nullptr;
    screenWidth = 1024;
    screenHeight = 768;
    gameState = GameState::Play;
}

void Game::Run() {
    InitSystems();

    sprite.Init(-1.0f, -1.0f, 1.0f, 1.0f);

    RunGameLoop();
}

void Game::InitSystems() {
    SDL_Init(SDL_INIT_EVERYTHING);

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

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void Game::RunGameLoop() {
    while (gameState != GameState::Exit) {
	ProcessInput();
	DrawGame();
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

    sprite.Draw();

    SDL_GL_SwapWindow(window);
}