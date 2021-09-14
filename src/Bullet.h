#pragma once

#include <glm/glm.hpp>
#include "SpriteBatch.h"

class Bullet {
public:
    Bullet(const glm::vec2& position, const glm::vec2& direction, float speed, int lifeTime);
    void Draw(SpriteBatch& spriteBatch);
    bool Update();
private:
    int lifeTime;
    float speed;
    glm::vec2 direction;
    glm::vec2 position;
};
