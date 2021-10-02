#pragma once

#include "../BoxCollider.h"
#include "../Vertex.h"
#include "../SpriteBatch.h"
#include "../Texture.h"
#include <glm/glm.hpp>

class Player {
public:
    Player();
    Player(class b2World* world, const glm::vec2& position, const glm::vec2& size,
	   const ColorRGBA8& color);
	
    void Update();
    void Draw(SpriteBatch& spriteBatch);

    const BoxCollider& GetBoxCollider() const { return boxCollider; }

private:
    BoxCollider boxCollider;
};
