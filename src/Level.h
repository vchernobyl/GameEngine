#pragma once

#include "SpriteBatch.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

const int TileSize = 64;


class Level {
public:
    Level(const std::string& fileName);
    void Draw();

    const glm::vec2& GetPlayerStartPosition() const { return playerStartPosition; }
    const std::vector<glm::vec2>& GetZombieStartPositions() const { return zombieStartPositions; }

private:
    std::vector<std::string> levelData;
    int numHumans;
    SpriteBatch spriteBatch;

    glm::vec2 playerStartPosition;
    std::vector<glm::vec2> zombieStartPositions;
};
