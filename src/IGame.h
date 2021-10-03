#pragma once

#include "Window.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include <memory>
#include <SDL/SDL.h>

class ScreenManager;
class IGameScreen;

class IGame {
public:
    IGame();
    virtual ~IGame() {}

    void Run();
    void ExitGame();

    virtual void OnInit() = 0;
    virtual void AddScreens() = 0;
    virtual void OnExit() = 0;

    virtual void OnSdlEvent(SDL_Event& event);

    float GetFps() const { return fps; }
    const InputManager& GetInputManager() const { return inputManager; }

protected:
    bool Init();

    virtual void Update();
    virtual void Draw();

    std::unique_ptr<ScreenManager> screenManager = nullptr;
    IGameScreen* currentScreen = nullptr;
    bool isRunning = false;
    float fps = 0;
    Window window;
    InputManager inputManager;
};
