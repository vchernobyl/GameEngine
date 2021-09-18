#pragma once

#include "Agent.h"
#include <vector>
#include <string>

class Human;
class Zombie : public Agent {
public:
    void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies) override;

};
