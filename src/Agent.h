#pragma once

#include "SpriteBatch.h"
#include "Vertex.h"
#include <glm/glm.hpp>

const float AgentSize = 60;

class Agent {
public:
    Agent();
    virtual ~Agent();
    virtual void Update() = 0;
    void Draw(SpriteBatch& spriteBatch);
    const glm::vec2& GetPosition() const { return position; }
protected:
    glm::vec2 position;
    float speed;
    Color color;
};
