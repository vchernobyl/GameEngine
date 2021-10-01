#include "App.h"
#include "GameplayScreen.h"

#include "../ScreenManager.h"

void App::OnInit() { }

void App::AddScreens() {
    gameplayScreen = std::make_unique<GameplayScreen>();
    screenManager->AddScreen(gameplayScreen.get());
    screenManager->SetScreen(gameplayScreen->GetScreenIndex());
}

void App::OnExit() { }
