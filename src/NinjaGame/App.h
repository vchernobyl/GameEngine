#pragma once

#include "../IGame.h"
#include "GameplayScreen.h"
#include <memory>

class App : public IGame {
    void OnInit() override;
    void AddScreens() override;
    void OnExit() override;
private:
    std::unique_ptr<GameplayScreen> gameplayScreen = nullptr;
};
