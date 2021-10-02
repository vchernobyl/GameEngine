#include "Player.h"
#include "../ResourceManager.h"

#include <box2d/box2d.h>

Player::Player() {
}

Player::Player(b2World* world, const glm::vec2& position, const glm::vec2& size,
	       const ColorRGBA8& color)
    : boxCollider(world,
		  position,
		  size,
		  ResourceManager::GetTexture("data/textures/Ninja/player.png"),
		  color, true) {
}

void Player::Update() {
}

void Player::Draw(SpriteBatch& spriteBatch) {
    boxCollider.Draw(spriteBatch);
}
