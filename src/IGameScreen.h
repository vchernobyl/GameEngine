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
    virtual void Build() = 0;
    virtual void Destroy() = 0;

    virtual void OnEntry() = 0;
    virtual void OnExit() = 0;

    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual int GetNextScreenIndex() const = 0;
    virtual int GetPreviousScreenIndex() const = 0;

    int GetScreenIndex() const { return screenIndex; }
    
protected:
    ScreenState currentState = ScreenState::None;
    class Game* game = nullptr;
private:
    int screenIndex = -1;
};
