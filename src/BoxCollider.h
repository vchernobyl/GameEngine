#pragma once

#include <glm/glm.hpp>
#include "Vertex.h"
#include "Texture.h"
#include <box2d/box2d.h>

class BoxCollider {
public:
    BoxCollider();
    BoxCollider(class b2World* world, const glm::vec2& position, const glm::vec2& size,
		Texture texture, const ColorRGBA8& color, bool fixedRotation = false,
		glm::vec4 uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    void Draw(class SpriteBatch& spriteBatch);

    glm::vec2 GetPosition() const;

    const glm::vec2& GetSize() const { return size; }

    const ColorRGBA8& GetColor() const { return color; }

    float GetAngle() const;

    void ApplyForceToCenter(const glm::vec2& force);
    void ApplyImpulse(const glm::vec2& impulse);

    b2Body* GetBody() { return body; }

    glm::vec2 GetCenter() const;

private:
    class b2Body* body = nullptr;
    class b2Fixture* fixture = nullptr;
    glm::vec2 size;
    glm::vec4 uv;
    ColorRGBA8 color;
    Texture texture;
};
