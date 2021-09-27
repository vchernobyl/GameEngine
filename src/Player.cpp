#include "Player.h"
#include "Zombie.h"
#include "ResourceManager.h"
#include <SDL/SDL.h>

Player::Player() {
}

void Player::Init(int speed, const glm::vec2& position, InputManager* inputManager) {
    this->speed = speed;
    this->position = position;
    this->color = ColorRGBA8(255, 255, 255, 255);
    this->inputManager = inputManager;
    this->textureID = ResourceManager::GetTexture("data/textures/player.png").id;
}

void Player::Update(const std::vector<std::string>& levelData,
		    std::vector<Human*>& humans,
		    std::vector<Zombie*>& zombies,
		    float deltaTime) {
    if (inputManager->IsKeyDown(SDLK_w)) {
	position.y += speed * deltaTime;
    }
    if (inputManager->IsKeyDown(SDLK_s)) {
	position.y -= speed * deltaTime;
    }
    if (inputManager->IsKeyDown(SDLK_d)) {
	position.x += speed * deltaTime;
    }
    if (inputManager->IsKeyDown(SDLK_a)) {
	position.x -= speed * deltaTime;
    }

    CollideWithLevel(levelData);
}
