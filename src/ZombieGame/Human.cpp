#include "Human.h"
#include "Zombie.h"

#include "../ResourceManager.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

Human::Human() {
}

Human::~Human() {
}

void Human::Init(float speed, glm::vec2 pos) {
    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    this->color = ColorRGBA8(255, 255, 255, 255);
    this->speed = speed;
    this->position = pos;
    this->textureID = ResourceManager::GetTexture("data/textures/human.png").id;

    direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    if (direction.length() == 0) direction = glm::vec2(1.0f, 0.0f);
    direction = glm::normalize(direction);
}

void Human::Update(const std::vector<std::string>& levelData,
		   std::vector<Human*>& humans,
		   std::vector<Zombie*>& zombies,
		   float deltaTime) {
    static std::mt19937 randEng(time(nullptr));
    static std::uniform_real_distribution<float> randRotate(-1.0f, 1.0f);
    
    position += direction * speed * deltaTime;

    if (frames == 130) {
	direction = glm::rotate(direction, randRotate(randEng));
	frames = 0;
    } else {
	frames++;
    }
    
    if (CollideWithLevel(levelData)) {
	direction = glm::rotate(direction, randRotate(randEng));
    }
}

