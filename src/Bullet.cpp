#include "Bullet.h"
#include "Vertex.h"
#include "ResourceManager.h"

Bullet::Bullet(const glm::vec2& position, const glm::vec2& direction, float speed, int lifeTime) {
    this->position = position;
    this->direction = direction;
    this->speed = speed;
    this->lifeTime = lifeTime;
}

void Bullet::Draw(SpriteBatch& spriteBatch) {
    ColorRGBA8 color;
    color.r = 255.0f;
    color.g = 255.0f;
    color.b = 255.0f;
    color.a = 255.0f;

    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Texture texture = ResourceManager::GetTexture("data/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

    glm::vec4 dest = glm::vec4(position.x, position.y, 30.0f, 30.0f);

    spriteBatch.Draw(dest, uv, texture.id, 0.0f, color);
}

bool Bullet::Update() {
    position += direction * speed;
//    lifeTime--;
    if (lifeTime <= 0) {
	return true;
    }
    return false;
}
