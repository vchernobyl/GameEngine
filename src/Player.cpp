#include "Player.h"
#include "Zombie.h"
#include <SDL/SDL.h>

Player::Player() {
}

void Player::Init(int speed, const glm::vec2& position, InputManager* inputManager) {
    this->speed = speed;
    this->position = position;
    this->color.r = 255;
    this->color.g = 255;
    this->color.b = 255;
    this->color.a = 255;
    this->inputManager = inputManager;
}

void Player::Update(const std::vector<std::string>& levelData,
		    std::vector<Human*>& humans,
		    std::vector<Zombie*>& zombies) {
    if (inputManager->IsKeyDown(SDLK_w)) {
	position.y += speed;
    }
    if (inputManager->IsKeyDown(SDLK_s)) {
	position.y -= speed;
    }
    if (inputManager->IsKeyDown(SDLK_d)) {
	position.x += speed;
    }
    if (inputManager->IsKeyDown(SDLK_a)) {
	position.x -= speed;
    }	

    CollideWithLevel(levelData);
}
