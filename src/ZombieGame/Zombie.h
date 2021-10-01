#pragma once

#include "Agent.h"

#include <vector>
#include <string>

class Human;
class Zombie : public Agent {
public:
    void Init(float speed, const glm::vec2& position);
    void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;
private:
    Human* GetNearestHuman(const std::vector<Human*> humans);
};
