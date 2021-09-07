#include "Game.h"
#include "Errors.h"
#include "ImageLoader.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

Game::Game() :
    window(nullptr),
    screenWidth(1024),
    screenHeight(768),
    gameState(GameState::Play),
    time(0.0f) {
}

void Game::Run() {
    InitSystems();

    sprite.Init(-1.0f, -1.0f, 2.0f, 2.0f);
    playerTexture = ImageLoader::LoadPNG("data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

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

    InitShaders();
}

void Game::InitShaders() {
    shader.Compile("data/shaders/ColorShading.vert", "data/shaders/ColorShading.frag");
    shader.AddAttribute("vertexPosition");
    shader.AddAttribute("vertexColor");
    shader.Link();
}

void Game::RunGameLoop() {
    while (gameState != GameState::Exit) {
	ProcessInput();
	time += 0.01f;
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

    shader.Use();
    GLuint timeLocation = shader.GetUniformLocation("time");
    glUniform1f(timeLocation, time);

    sprite.Draw();
    shader.Unuse();

    SDL_GL_SwapWindow(window);
}
