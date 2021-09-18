#pragma once

#include "SpriteBatch.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

const float AgentSize = 60;

class Zombie;
class Human;

class Agent {
public:
    Agent();
    virtual ~Agent();

    virtual void Update(const std::vector<std::string>& levelData,
			std::vector<Human*>& humans,
			std::vector<Zombie*>& zombies) = 0;

    void CollideWithLevel(const std::vector<std::string>& levelData);

    void Draw(SpriteBatch& spriteBatch);
    const glm::vec2& GetPosition() const { return position; }
protected:
    void CheckTilePosition(const std::vector<std::string>& levelData,
			   std::vector<glm::vec2>& collideTilePositions,
			   float x, float y);
    void CollideWithTile(glm::vec2 tilePosition);
    glm::vec2 position;
    float speed;
    Color color;
};
