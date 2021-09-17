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
private:
    std::vector<std::string> levelData;
    int numHumans;
    SpriteBatch spriteBatch;

    glm::ivec2 playerStartPosition;
    std::vector<glm::ivec2> zombieStartPositions;
};
