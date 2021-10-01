#include "GameplayScreen.h"
#include "../IGame.h"
#include <iostream>
#include <GL/glew.h>

void GameplayScreen::Build() { }

void GameplayScreen::Destroy() { }

void GameplayScreen::OnEntry() {
    std::cout << "OnEntry\n";
}

void GameplayScreen::OnExit() { }

void GameplayScreen::Update() {
    std::cout << "Update\n";
    ProcessInput();
}

void GameplayScreen::Draw() {
    std::cout << "Draw\n";
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int GameplayScreen::GetNextScreenIndex() const {
    return -1;
}

int GameplayScreen::GetPreviousScreenIndex() const {
    return -1;
}

void GameplayScreen::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	game->OnSdlEvent(event);
    }
}
