#include "IGame.h"
#include "Engine.h"
#include "Timing.h"
#include "ScreenManager.h"
#include "IGameScreen.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

IGame::IGame() {
    screenManager = std::make_unique<ScreenManager>(this);
}

void IGame::Run() {
    if (!Init()) {
	return;
    }

    FpsLimiter limiter;
    limiter.SetMaxFPS(60.0f);

    isRunning = true;
    while (isRunning) {
	limiter.Begin();

	inputManager.Update();
	Update();
	Draw();

	fps = limiter.End();
    }
}

void IGame::ExitGame() {
    currentScreen->OnExit();

    if (screenManager) {
	screenManager->Destroy();
	screenManager.reset();
    }

    isRunning = false;
}

bool IGame::Init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    window.Create("Default", 1024, 768, 0);

    OnInit();
    AddScreens();

    currentScreen = screenManager->GetCurrentScreen();
    currentScreen->OnEntry();
    currentScreen->SetRunning();

    return true;
}

void IGame::Update() {
    if (currentScreen) {
	switch (currentScreen->GetState()) {
	case ScreenState::Running:
	    currentScreen->Update();
	    break;
	case ScreenState::ChangeNext:
	    currentScreen->OnExit();
	    currentScreen = screenManager->MoveToNext();
	    if (currentScreen) {
		currentScreen->SetRunning();
		currentScreen->OnEntry();
	    }
	    break;
	case ScreenState::ChangePrevious:
	    currentScreen->OnExit();
	    currentScreen = screenManager->MoveToPrevious();
	    if (currentScreen) {
		currentScreen->SetRunning();
		currentScreen->OnEntry();
	    }
	    break;
	case ScreenState::ExitApplication:
	    ExitGame();
	    break;
	default:
	    break;
	}
    } else {
	ExitGame();
    }
}

void IGame::Draw() {
    glViewport(0, 0, window.GetScreenWidth(), window.GetScreenHeight());
    if (currentScreen && currentScreen->GetState() == ScreenState::Running) {
	currentScreen->Draw();
    }
    window.SwapBuffer();
}

void IGame::OnSdlEvent(SDL_Event& event) {
    switch (event.type) {
    case SDL_QUIT:
	ExitGame();
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
