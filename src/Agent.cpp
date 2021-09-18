#include "Agent.h"
#include "ResourceManager.h"
#include "Level.h"
#include <algorithm>

Agent::Agent() {
}

Agent::~Agent() {
}

void Agent::CollideWithLevel(const std::vector<std::string>& levelData) {
    std::vector<glm::vec2> collideTilePositions;

    CheckTilePosition(levelData, collideTilePositions, position.x, position.y);
    CheckTilePosition(levelData, collideTilePositions, position.x + AgentSize, position.y);
    CheckTilePosition(levelData, collideTilePositions, position.x, position.y + AgentSize);
    CheckTilePosition(levelData, collideTilePositions, position.x + AgentSize, position.y + AgentSize);

    for (auto& tilePosition : collideTilePositions) {
	CollideWithTile(tilePosition);
    }
}

void Agent::Draw(SpriteBatch& spriteBatch) {
    static int textureID = ResourceManager::GetTexture("data/textures/circle.png").id;
    spriteBatch.Draw(glm::vec4(position.x, position.y, AgentSize, AgentSize),
		     glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		     textureID,
		     0.0f,
		     color);
}

void Agent::CheckTilePosition(const std::vector<std::string>& levelData,
			      std::vector<glm::vec2>& collideTilePositions,
			      float x, float y) {
    glm::vec2 cornerPos = glm::vec2(floor(x / (float)TileSize),
				    floor(y / (float)TileSize));

    if (levelData[cornerPos.y][cornerPos.x] != '.') {
	collideTilePositions.push_back(cornerPos * (float)TileSize + glm::vec2((float)TileSize / 2.0f));
    }    
}

void Agent::CollideWithTile(glm::vec2 tilePosition) {
    const float agentRadius = (float)AgentSize / 2.0f;
    const float tileRadius = (float)TileSize / 2.0f;
    const float minDistance = agentRadius + tileRadius;

    glm::vec2 centerPlayerPos = position + glm::vec2(agentRadius);
    glm::vec2 distVec = centerPlayerPos - tilePosition;
    float xDepth = minDistance - abs(distVec.x);
    float yDepth = minDistance - abs(distVec.y);

    if (xDepth > 0 || yDepth > 0) {
	if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
	    if (distVec.x < 0) {
		position.x -= xDepth;
	    } else {
		position.x += xDepth;
	    }
	} else {
	    if (distVec.y < 0) {
		position.y -= yDepth;
	    } else {
		position.y += yDepth;
	    }
	}
    }
}