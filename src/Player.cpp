#include "Player.h"
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

void Player::Update() {
    if (inputManager->IsKeyPressed(SDLK_w)) {
	position.y += speed;
    } else if (inputManager->IsKeyPressed(SDLK_s)) {
	position.y -= speed;
    } else if (inputManager->IsKeyPressed(SDLK_d)) {
	position.x += speed;
    } else if (inputManager->IsKeyPressed(SDLK_a)) {
	position.x -= speed;
    }	
}
