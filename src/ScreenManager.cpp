#include "ScreenManager.h"
#include "IGame.h"
#include "IGameScreen.h"

ScreenManager::ScreenManager(IGame* game) : game(game) {
}

ScreenManager::~ScreenManager() {
    Destroy();
}

IGameScreen* ScreenManager::MoveToNext() {
    IGameScreen* currentScreen = GetCurrentScreen();
    if (int nextIndex = currentScreen->GetNextScreenIndex(); nextIndex != -1) {
	currentScreenIndex = nextIndex;
    }
    return GetCurrentScreen();
}

IGameScreen* ScreenManager::MoveToPrevious() {
    IGameScreen* currentScreen = GetCurrentScreen();
    if (int previousIndex = currentScreen->GetPreviousScreenIndex(); previousIndex != -1) {
	currentScreenIndex = previousIndex;
    }
    return GetCurrentScreen();
}

void ScreenManager::SetScreen(int screenIndex) {
    currentScreenIndex = screenIndex;
}

void ScreenManager::AddScreen(IGameScreen* screen) {
    screen->screenIndex = screens.size();
    screens.push_back(screen);
    screen->Build();
    screen->SetParentGame(game);
}

void ScreenManager::Destroy() {
    for (size_t i = 0; i < screens.size(); i++) {
	screens[i]->Destroy();
    }
    screens.resize(0);
    currentScreenIndex = -1;
}

IGameScreen* ScreenManager::GetCurrentScreen() const {
    if (currentScreenIndex == -1) {
	return nullptr;
    }
    return screens[currentScreenIndex];
}
