#pragma once

#include <glm/glm.hpp>
#include "Vertex.h"
#include "Texture.h"

class BoxCollider {
public:
    BoxCollider();
    BoxCollider(class b2World* world, const glm::vec2& position, const glm::vec2& size,
		Texture texture, const ColorRGBA8& color, bool fixedRotation = false);

    void Draw(class SpriteBatch& spriteBatch);

    glm::vec2 GetPosition() const;
    const glm::vec2& GetSize() const { return size; }
    const ColorRGBA8& GetColor() const { return color; }
    float GetAngle() const;

private:
    class b2Body* body = nullptr;
    class b2Fixture* fixture = nullptr;
    glm::vec2 size;
    ColorRGBA8 color;
    Texture texture;
};
