#pragma once

#include <vector>

class IGame;
class IGameScreen;

class ScreenManager {
public:
    ScreenManager(IGame* game);
    ~ScreenManager();

    IGameScreen* MoveToNext();
    IGameScreen* MoveToPrevious();

    void SetScreen(int screenIndex);
    void AddScreen(IGameScreen* screen);
    void Destroy();

    IGameScreen* GetCurrentScreen() const;

protected:
    IGame* game = nullptr;
    std::vector<IGameScreen*> screens;
    int currentScreenIndex = -1;
};
