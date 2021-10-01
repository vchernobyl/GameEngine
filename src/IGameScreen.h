#pragma once

enum class ScreenState {
    None,
    Running,
    ExitApplication,
    ChangeNext,
    ChangePrevious
};

class IGameScreen {
public:
    virtual ~IGameScreen() {}

    virtual void Build() = 0;
    virtual void Destroy() = 0;

    virtual void OnEntry() = 0;
    virtual void OnExit() = 0;

    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual int GetNextScreenIndex() const = 0;
    virtual int GetPreviousScreenIndex() const = 0;

    int GetScreenIndex() const { return screenIndex; }
    void SetParentGame(class IGame* game) { this->game = game; }

    ScreenState GetState() const { return currentState; }
    void SetRunning() { currentState = ScreenState::Running; }
    
protected:
    ScreenState currentState = ScreenState::None;
    class IGame* game = nullptr;
private:
    int screenIndex = -1;
};
