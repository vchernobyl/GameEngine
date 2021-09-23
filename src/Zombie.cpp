#include "Zombie.h"
#include "Human.h"

void Zombie::Init(float speed, const glm::vec2& position) {
    this->speed = speed;
    this->position = position;
    this->color = ColorRGBA8(0, 160, 0, 255);
}

void Zombie::Update(const std::vector<std::string>& levelData,
		    std::vector<Human*>& humans,
		    std::vector<Zombie*>& zombies,
		    float deltaTime) {
    CollideWithLevel(levelData);
    Human* closestHuman = GetNearestHuman(humans);

    if (closestHuman) {
	glm::vec2 target = closestHuman->GetPosition() - position;
	if (target.length() > 0.0f) {
	    glm::vec2 direction = glm::normalize(closestHuman->GetPosition() - position);
	    position += direction * 1.5f * deltaTime;
	}
    }
}

Human* Zombie::GetNearestHuman(const std::vector<Human*> humans) {
    Human* closestHuman = nullptr;
    float smallestDistance = 99999999.0f;

    for (auto human : humans) {
	glm::vec2 distVec = human->GetPosition() - position;
	float distance = glm::length(distVec);
	if (distance < smallestDistance) {
	    smallestDistance = distance;
	    closestHuman = human;
	}
    }

    return closestHuman;
}
