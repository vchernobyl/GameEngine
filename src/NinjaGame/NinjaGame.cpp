#include "NinjaGame.h"
#include "GameplayScreen.h"

#include "../ScreenManager.h"

void NinjaGame::OnInit() { }

void NinjaGame::AddScreens() {
    gameplayScreen = std::make_unique<GameplayScreen>(&window);
    screenManager->AddScreen(gameplayScreen.get());
    screenManager->SetScreen(gameplayScreen->GetScreenIndex());
}

void NinjaGame::OnExit() { }
