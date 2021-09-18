#pragma once

#include "Human.h"
#include "InputManager.h"

class Player : public Human {
public:
    Player();
    void Init(int speed, const glm::vec2& position, InputManager* inputManager);
    void Update() override;
private:
    InputManager* inputManager;
};

