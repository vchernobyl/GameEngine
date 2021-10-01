#pragma once

#include "../IGameScreen.h"

class GameplayScreen : public IGameScreen {
public:
    void Build() override;
    void Destroy() override;

    void OnEntry() override;
    void OnExit() override;

    void Update() override;
    void Draw() override;

    int GetNextScreenIndex() const override;
    int GetPreviousScreenIndex() const override;

private:
    void ProcessInput();
};
