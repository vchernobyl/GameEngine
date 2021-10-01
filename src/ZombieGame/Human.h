#pragma once

#include "Agent.h"
#include <string>
#include <vector>

class Zombie;
class Human : public Agent {
public:
    Human();
    virtual ~Human();
    void Init(float speed, glm::vec2 pos);
    void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;
private:
    int frames = 0;
};
