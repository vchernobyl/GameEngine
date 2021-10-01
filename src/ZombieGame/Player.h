#pragma once

#include "Human.h"

#include "../InputManager.h"

#include <string>
#include <vector>

class Zombie;
class Player : public Human {
public:
    Player();
    void Init(int speed, const glm::vec2& position, InputManager* inputManager);
    void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

private:
    InputManager* inputManager;
};

